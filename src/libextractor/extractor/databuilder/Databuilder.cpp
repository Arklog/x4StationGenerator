//
// Created by pierre on 7/9/26.
//

#include "Databuilder.hpp"

namespace extractor::databuilder {
    Databuilder::Modules::Modules(AggregateStore &&store, std::set<common::types::Ware::ware_id> &used_wares) {
        for (auto &[_, value]: store.modules.modules) {
            build_production_module(value, store, used_wares);
        }

        for (auto &[_, value]: store.habitats.modules) {
            build_habitat_module(value, store, used_wares);
        }

        for (auto &[_, value]: store.docks.modules) {
            if (value.module_class == "pier")
                build_pier(value, store, used_wares);
            else
                build_docks(value, store, used_wares);
        }
    }

    void Databuilder::Modules::build_production_module(ProductionModule &module, AggregateStore &store,
                                                       ware_whitelist &  used_wares) {
        common::types::module::ProductionModule tmp{};
        build_module(tmp, module, store, used_wares);
        tmp.required_workforce = module.workforce_max;
        tmp.production_method  = std::move(module.production_method);

        for (auto &ware_id: module.wares_produced) {
            try {
                auto &ware            = store.wares.by_id.at(ware_id);
                auto &ware_production = ware.production.at(tmp.production_method);

                if (ware_production.amount == 0)
                    continue;
                used_wares.insert(ware_id);

                tmp.wares_produced.emplace(ware_id, ware_production.amount);
                for (auto &[required_id, required_amount]: ware_production.wares_required) {
                    if (required_amount == 0)
                        continue;
                    used_wares.insert(required_id);
                    tmp.wares_required.emplace(required_id, required_amount);
                }
            } catch (const std::out_of_range &e) {
                spdlog::error("No match for ware {} with production method {}: \nError originating from module {}",
                              ware_id, tmp.production_method, tmp.id);
            }
        }
        if (!tmp.wares_produced.empty())
            this->productions.push_back(std::move(tmp));
    }

    void Databuilder::Modules::
    build_habitat_module(Habitat &habitat, AggregateStore &store, ware_whitelist &used_wares) {
        try {
            common::types::module::Habitat tmp{};
            tmp.capacity = habitat.capacity;
            tmp.race     = std::move(habitat.workforce_race);
            build_module(tmp, habitat, store, used_wares);
            this->habitats.push_back(std::move(tmp));
        } catch (const std::out_of_range &e) {
            spdlog::error("No match for habitat {}", habitat.macro);
        }
    }

    void Databuilder::Modules::build_docks(Dock &dock, AggregateStore &store, ware_whitelist &used_wares) {
        try {
            common::types::module::Dock tmp{};
            build_module(tmp, dock, store, used_wares);
            this->docks.push_back(std::move(tmp));
        } catch (const std::out_of_range &e) {
            spdlog::error("No match for dock {}", dock.macro);
        }
    }

    void Databuilder::Modules::build_pier(Dock &dock, AggregateStore &store, ware_whitelist &used_wares) {
        try {
            common::types::module::Pierr tmp{};
            build_module(tmp, dock, store, used_wares);
            this->piers.push_back(std::move(tmp));
        } catch (const std::out_of_range &e) {
            spdlog::error("No match for pier {}", dock.macro);
        }
    }

    Databuilder::Databuilder(AggregateStore &&store) :
    modules{},
    wares{} {
        ware_whitelist whitelist;
        this->modules = Modules{std::move(store), whitelist};
        for (auto &wareid: whitelist) {
            try {
                auto &              ware      = store.wares.by_id.at(wareid);
                auto &              waregroup = store.waregroups.waregroups.at(ware.group);
                common::types::Ware tmp{};
                tmp.id    = std::move(ware.id);
                tmp.name  = std::move(ware.name);
                tmp.price = common::types::Price{.max = ware.price.max, .min = ware.price.min, .avg = ware.price.avg};
                tmp.group = waregroup.name.value();
                tmp.tier  = waregroup.tier.value().value();
                this->wares.push_back(std::move(tmp));
            } catch (const std::out_of_range &e) {
                spdlog::error("No match for ware {}", wareid);
            }
        }
    }
} // extractor
