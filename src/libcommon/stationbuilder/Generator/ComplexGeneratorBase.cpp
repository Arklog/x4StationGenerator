//
// Created by pierre on 7/20/25.
//

#include "ComplexGeneratorBase.hpp"
#include "data/WareModuleAndWorkforce.hpp"
#include "utils/FmtVectorFormatter.hpp"

#include "spdlog/spdlog.h"

namespace common::stationbuilder::generator {
    bool ComplexGeneratorBase::_done(const t_target_container &targets,
                                     t_target_container &      current_state,
                                     t_x4_complex &            modules) const {
        // check primary is produced enougth
        auto primary_check = [current_state](const WareTarget *primary) {
            auto current_target = current_state.getPrimaryTarget(primary->ware_id);
            return current_target->prodution >= primary->prodution;
        };

        // check no produced secondary is in deficit
        auto secondary_check = [this](const WareTarget *target) {
            const auto &store = this->store_;
            return target->prodution > 0 || !store.wares.by_id.at(target->ware_id)->produced;
        };

        bool primary_ok   = std::ranges::all_of(targets.getPrimaryTargets(), primary_check);
        bool secondary_ok = std::ranges::all_of(current_state.getSecondaryTargets(), secondary_check);

        return primary_ok && secondary_ok;
    }

    void ComplexGeneratorBase::_step(const t_target_container &targets, t_target_container &current_state,
                                     t_x4_complex &            modules) {
        const auto &next        = _nextTarget(targets, current_state, modules);
        const auto &next_module = store_.production.by_id.at(next->source_module);
        const auto &production  = next_module->wares_produced;

        // add all wares produced + adjust for workforce and sun factor
        std::ranges::for_each(production, [this](const auto &v) {
            auto &ware_id = v.first;
            auto  data    = v.second;

            if (settings_.workforce_enables)
                data.amount *= data.work;
            data.amount *= 1.0f + (settings_.sunlight * data.sun);
            _updateCurrentProduction(ware_id, data.amount);
        });

        // add all wares consumed
        std::ranges::for_each(next_module->wares_required, [this](const auto &v) {
            auto &ware_id = v.first;
            auto  amount  = v.second;

            _updateCurrentProduction(ware_id, amount);
        });

        modules.push_back(next_module->module.get().id);
        if (settings_.workforce_enables)
            _add_workforce(next_module->required_workforce, modules);
    }

    WareTarget *
    ComplexGeneratorBase::_nextTarget(const t_target_container &targets,
                                      t_target_container &      current_state,
                                      t_x4_complex &            modules) {
        spdlog::debug("determining next target");
        std::vector<t_ware_id> ids{};

        auto all_wares = current_state.getPrimaryAndSecondaryTargets();
        // Get all potential next targets
        for (const auto &ware: all_wares) {
            const auto &key = ware->ware_id;

            if (!store_.production.producing.contains(key))
                continue;

            auto is_base_target = targets.isPrimaryTarget(key);
            auto is_done
                    = is_base_target
                          ? targets.getPrimaryTarget(key)->prodution <= ware->prodution
                          : ware->prodution >= 0;

            if (!is_done)
                ids.push_back(key);
        }

        std::ranges::sort(ids, [current_state, this](const t_ware_id &a,
                                                     const t_ware_id &b) {
            const auto &a_ware = current_state.getTarget(a);
            const auto &b_ware = current_state.getTarget(b);

            auto a_neg = b_ware->prodution < 0;
            auto b_neg = a_ware->prodution < 0;

            if (a_neg || b_neg)
                return a_ware->prodution < b_ware->prodution;

            const auto &wares       = this->store_.wares.by_id;
            const auto  a_ware_data = wares.at(a_ware->ware_id);
            const auto  b_ware_data = wares.at(a_ware->ware_id);

            return a_ware_data->tier > b_ware_data->tier;
        });

        spdlog::debug("next target {}", ids.at(0));
        return current_state.getTarget(ids[0]);
    }

    void ComplexGeneratorBase::_updateCurrentProduction(const t_ware_id &ware_id,
                                                        long int         value) {
        if (!store_.production.producing.contains(ware_id)) {
            spdlog::info("ware {} is not produced, skipping", ware_id);
            return;
        }
        spdlog::debug("update current production {}", ware_id);

        auto is_produced = this->current_production_.isPrimaryTarget(ware_id)
                           || this->current_production_.isSecondaryTarget(ware_id);

        // If ware is not in the list of produced ware
        // Happens if required to produce another ware
        if (is_produced) {
            this->current_production_.getTarget(ware_id)->prodution += value;
            return;
        }

        spdlog::debug("adding ware in list {}", ware_id);
        this->current_production_.setSecondaryTarget(ware_id);
        auto target       = this->current_production_.getSecondaryTarget(ware_id);
        target->prodution = value;
    }

    void ComplexGeneratorBase::_add_workforce(size_t amount, t_x4_complex &modules) {
        const auto habitat = store_.habitats.by_id.at(settings_.workforce_module);
        const auto race    = store_.workforce.by_id.at(habitat->race);

        std::ranges::for_each(race->getConsumption(amount), [this](auto &item) {
            _updateCurrentProduction(item.first, -item.second);
        });

        workforce_current_ += amount;
        while (workforce_max_ < workforce_max_) {
            workforce_max_ += habitat->capacity;
            modules.push_back(habitat->module.get().id);
        }
    }

    ComplexGeneratorBase::ComplexGeneratorBase(const Settings &            settings,
                                               const data::Store &         store,
                                               utils::WareTargetContainer &targets) :
    targets_(targets),
    current_production_{store},
    settings_(settings),
    store_(store) {
        // Copy all targets production methods
        for (const auto &target: targets.getTargets()) {
            auto current_target           = current_production_.getTarget(target.ware_id);
            current_target->source_module = target.source_module;
        }

        // Set primary targets
        auto primary_targets = targets_.getPrimaryTargets();
        for (const auto &target: primary_targets) {
            current_production_.setPrimaryTarget(target->ware_id);
            current_production_.getPrimaryTarget(target->ware_id)->prodution = 0;
        }
    }

    Complex ComplexGeneratorBase::build() {
        Complex result{};
        workforce_max_     = 0;
        workforce_current_ = 0;
        spdlog::info("Starting complex generation");

        current_step_ = 0;
        sunlight_     = this->settings_.sunlight;
        while (!_done(this->targets_, this->current_production_, result.complex)) {
            _step(this->targets_, this->current_production_, result.complex);
            ++current_step_;
        }

        spdlog::info("Complex generated");
        result.name          = settings_.name;
        result.wares         = current_production_;
        result.sun           = settings_.sunlight;
        result.workforce     = workforce_current_;
        result.workforce_max = workforce_max_;
        return result;
    }

    const ComplexGeneratorBase::t_target_container &
    ComplexGeneratorBase::getCurrentProduction() const {
        return current_production_;
    }
}
