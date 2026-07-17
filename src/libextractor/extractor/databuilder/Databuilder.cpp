//
// Created by pierre on 7/9/26.
//

#include "Databuilder.hpp"
#include "extractor/utils.hpp"

namespace extractor::databuilder {
    Databuilder::Modules::Modules(AggregateStore &&store, std::set<common::types::Ware::ware_id> &used_wares) {
        for (auto &[_, value]: store.modules.modules) {
            build_production_module(value, store, used_wares);
        }

        for (auto &[_, value]: store.habitats.modules) {
            build_habitat_module(value, store, used_wares);
        }

        for (auto &[_, value]: store.storages.modules) {
            build_storage_module(value, store, used_wares);
        }

        for (auto &[_, value]: store.docks.modules) {
            if (value.module_class == "pier")
                build_pier(value, store, used_wares);
            else
                build_docks(value, store, used_wares);
        }
    }

    void Databuilder::Modules::build_production_module(ProductionModule &module, AggregateStore &store,
                                                       t_ware_whitelist &ware_whitelist) {
        common::types::module::ProductionModule finished_module{};
        build_module(finished_module, module, store, ware_whitelist);
        finished_module.required_workforce = module.workforce_max;

        for (auto &[ware_id, method]: module.wares_produced) {
            try {
                auto & ware            = store.wares.by_id.at(ware_id);
                auto & ware_production = ware.production.at(method);
                double time_factor     = 3600.0f / (ware_production.time * module.wares_produced.size());

                if (ware_production.amount == 0)
                    continue;
                ware_whitelist.insert(ware_id);

                common::types::module::ProductionModule::ProducedWare ware_production_data{
                    .amount = static_cast<size_t>(std::floor(ware_production.amount * time_factor)),
                    .work   = ware_production.effects["work"] + 1.0f,
                    .sun    = ware_production.effects["sun"]
                };
                finished_module.wares_produced.emplace(ware_id, std::move(ware_production_data));

                for (auto &[required_id, required_amount]: ware_production.wares_required) {
                    if (required_amount == 0)
                        continue;
                    ware_whitelist.insert(required_id);
                    auto &required_amount_total = finished_module.wares_required[required_id];
                    required_amount_total       -= static_cast<long long>(std::ceil(required_amount * time_factor));
                }
            } catch (const std::out_of_range &e) {
                spdlog::error("No match for ware {} with production method {}: \nError originating from module {}",
                              ware_id, finished_module.production_method, finished_module.module.value().id);
            }
        }
        if (finished_module.wares_produced.empty())
            spdlog::warn("Module {} has no wares produced", finished_module.module.value().id);
        else
            this->productions.push_back(std::move(finished_module));
    }

    void Databuilder::Modules::
    build_habitat_module(Habitat &habitat, AggregateStore &store, t_ware_whitelist &used_wares) {
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

    void Databuilder::Modules::
    build_storage_module(Storage &storage, AggregateStore &store, t_ware_whitelist &used_wares) {
        try {
            common::types::module::Storage tmp{};
            build_module(tmp, storage, store, used_wares);
            tmp.type     = std::move(storage.storage_type);
            tmp.capacity = storage.storage_max;
            this->storage.push_back(std::move(tmp));
        } catch (const std::out_of_range &e) {
            spdlog::error("No match for storage {}", storage.macro);
        }
    }

    void Databuilder::Modules::build_docks(Dock &dock, AggregateStore &store, t_ware_whitelist &used_wares) {
        try {
            common::types::module::Dock tmp{};
            build_module(tmp, dock, store, used_wares);
            this->docks.push_back(std::move(tmp));
        } catch (const std::out_of_range &e) {
            spdlog::error("No match for dock {}", dock.macro);
        }
    }

    void Databuilder::Modules::build_pier(Dock &dock, AggregateStore &store, t_ware_whitelist &used_wares) {
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
        t_ware_whitelist whitelist;
        this->modules    = Modules{std::move(store), whitelist};
        auto is_produced = [this](const common::types::Ware::ware_id &id) -> bool {
            for (const auto &module: this->modules.productions) {
                using v_type = decltype(module.wares_produced)::value_type;
                auto v       = std::find_if(module.wares_produced.begin(), module.wares_produced.end(),
                                      [&id](const v_type &n) {
                                          return id == n.first;
                                      });
                if (v != module.wares_produced.end()) {
                    return true;
                }
            }

            return false;
        };

        for (auto &wareid: whitelist) {
            try {
                auto &ware      = store.wares.by_id.at(wareid);
                auto &waregroup = store.waregroups.waregroups.at(ware.group);

                common::types::Ware tmp{};
                tmp.id = std::move(ware.id);
                tmp.name = std::move(ware.name);
                tmp.price = common::types::Price{.max = ware.price.max, .min = ware.price.min, .avg = ware.price.avg};
                tmp.group = waregroup.id.value();
                tmp.group_name = waregroup.name.value();
                tmp.tier = waregroup.tier.value().value();
                tmp.produced = is_produced(ware.id);

                this->wares.push_back(std::move(tmp));
            } catch (const std::out_of_range &e) {
                spdlog::error("No match for ware {}", wareid);
            }
        }
    }
} // extractor
