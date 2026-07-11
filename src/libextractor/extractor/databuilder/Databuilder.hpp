//
// Created by pierre on 7/9/26.
//

#ifndef X4STATIONGENERATOR__DATABUILDER_HPP
#define X4STATIONGENERATOR__DATABUILDER_HPP
#include <vector>

#include "common/types/module/Dock.hpp"
#include "common/types/module/Habitat.hpp"
#include "common/types/module/Pierr.hpp"
#include "extractor/databuilder/AggregateStore.hpp"
#include "common/types/module/ProductionModule.hpp"
#include "common/types/module/Storage.hpp"

namespace extractor::databuilder {
    using ware_whitelist = std::set<common::types::Ware::ware_id>;

    struct Databuilder {
        struct Modules {
            Modules() = default;

            Modules(AggregateStore &&store, ware_whitelist &used_wares);

            std::vector<common::types::module::ProductionModule> productions;
            std::vector<common::types::module::Habitat>          habitats;
            std::vector<common::types::module::Storage>          storage;
            std::vector<common::types::module::Dock>             docks;
            std::vector<common::types::module::Pierr>            piers;

        private:
            template<typename T, std::derived_from<databuilder::ModuleBase> J>
            void build_module(T &m, J &module_aggregate, AggregateStore &store, ware_whitelist &used_wares) {
                auto &module = m.module.value();
                try {
                    auto &module_ware = store.wares.by_ref.at(module_aggregate.macro);
                    module.id         = module_aggregate.macro;
                    module.price      = common::types::Price{
                        .max = module_ware.price.max, .min = module_ware.price.min, .avg = module_ware.price.avg
                    };
                    auto &module_cost = module_ware.production.begin()->second.wares_required;
                    for (auto &[ware_id, amount]: module_cost) {
                        used_wares.insert(ware_id);
                        module.cost.emplace(std::move(ware_id), amount);
                    }
                } catch (const std::out_of_range &e) {
                    spdlog::error("No match for module {} build cost", module.id);
                    throw;
                }
            }

            void build_production_module(ProductionModule &module, AggregateStore &store, ware_whitelist &used_wares);

            void build_habitat_module(Habitat &habitat, AggregateStore &store, ware_whitelist &used_wares);

            void build_docks(Dock &dock, AggregateStore &store, ware_whitelist &used_wares);

            void build_pier(Dock &dock, AggregateStore &store, ware_whitelist &used_wares);
        };

        Databuilder(AggregateStore &&store);

        Modules                          modules;
        std::vector<common::types::Ware> wares;

    private:
    };
} // extractor

#endif //X4STATIONGENERATOR__DATABUILDER_HPP
