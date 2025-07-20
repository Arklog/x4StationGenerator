//
// Created by pierre on 7/20/25.
//

#include "StationBuilder/Generator/ComplexGeneratorBase.hpp"

#include "Data/WaresAndModules.hpp"

#include "spdlog/spdlog.h"

bool ComplexGeneratorBase::_done(const t_target_map &targets, t_target_map &current_state,
                                 t_x4_complex &modules) const {
    for (const auto &iter: current_state) {
        const auto &key = iter.first;
        const auto &ware = iter.second;

        bool is_neg = ware.prodution < 0;
        if (is_neg && isWareProduced(ware.ware_id))
            return false;

        bool is_target = targets.contains(key);
        if (is_target && ware.prodution < targets.at(ware.ware_id).prodution)
            return false;
    }

    return true;
}

void ComplexGeneratorBase::_step(const t_target_map &targets, t_target_map &current_state, t_x4_complex &modules) {
    const auto &wares = getWares();
    auto ware = this->_nextTarget(targets, current_state, modules);

    const auto &module_to_add = getModule(ware.ware_id, ware.production_method_id);

}

WareTarget &ComplexGeneratorBase::_nextTarget(const t_target_map &targets, t_target_map &current_state,
                                              t_x4_complex &modules) {
    spdlog::debug("determining next target");
    std::vector<t_ware_id> ids{};

    ids.reserve(current_state.size());

    // Get all potential next targets
    for (const auto &ware_checked: current_state) {
        if (!isWareProduced(ware_checked.first))
            continue;

        auto const &key = ware_checked.first;
        auto const &ware = ware_checked.second;
        auto is_base_target = targets.contains(key);
        auto is_done = is_base_target ? targets.at(key).prodution >= ware.prodution : ware.prodution < 0;

        if (!is_done)
            ids.push_back(key);
    }

    std::ranges::sort(ids, [current_state](const t_ware_id &a, const t_ware_id &b) {
        const auto &a_ware = current_state.at(a);
        const auto &b_ware = current_state.at(b);

        auto a_neg = b_ware.prodution < 0;
        auto b_neg = a_ware.prodution < 0;

        if (a_neg || b_neg)
            return a_ware.prodution < b_ware.prodution;

        const auto &wares = getWares();
        const auto a_ware_data = wares.at(a_ware.ware_id);
        const auto b_ware_data = wares.at(a_ware.ware_id);

        return a_ware_data->group.tier > b_ware_data->group.tier;
    });

    return current_state[ids[0]];
}

void ComplexGeneratorBase::_updateCurrentProduction(const t_ware_id &ware_id, long int value) {
    auto is_produced = this->_current_production.contains(ware_id);

    if (is_produced) {
        this->_current_production.at(ware_id).prodution += value;
        return;
    }

    this->_current_production.emplace(std::pair<t_ware_id, WareTarget>{
        ware_id, WareTarget{.ware_id = ware_id, .production_method_id = "Universal", .prodution = value}
    });
}

ComplexGeneratorBase::ComplexGeneratorBase(const t_target_list &targets): _targets{}, _current_production{} {
    for (const auto target: targets) {
        _targets[target->ware_id] = *target;
        _current_production[target->ware_id] = _current_production[target->ware_id];
        _current_production[target->ware_id].prodution = 0;
    }
}

t_x4_complex ComplexGeneratorBase::build() {
    t_x4_complex result{};
    spdlog::info("staring complex generation");

    while (!_done(this->_targets, this->_current_production, result)) {
        spdlog::info("entering new step in complex generation");
        _step(this->_targets, this->_current_production, result);
    }

    spdlog::info("finished complex generation");
    return result;
}
