//
// Created by pierre on 3/31/26.
//

#ifndef X4STATIONGENERATOR__STORE_HPP
#define X4STATIONGENERATOR__STORE_HPP

#include "Aggregate.hpp"
#include "common/types/Workforce.hpp"
#include "common/types/module/Dock.hpp"
#include "common/types/module/Habitat.hpp"
#include "common/types/module/ModuleWrapper.hpp"
#include "common/types/module/Pierr.hpp"
#include "common/types/module/ProductionModule.hpp"
#include "common/types/module/Storage.hpp"

namespace common::data {
    struct Store {
        using DockAggregate       = Aggregate<types::module::Dock>;
        using PierAggregate       = Aggregate<types::module::Pierr>;
        using HabitatAggregate    = Aggregate<types::module::Habitat>;
        using StorageAggregate    = Aggregate<types::module::Storage>;
        using ProductionAggregate = Aggregate<types::module::ProductionModule>;
        using AllModulesAggregate = Aggregate<types::module::ModuleWrapper>;
        using WareAggregate       = Aggregate<types::Ware>;
        using WorkforceAggregate  = Aggregate<types::Workforce>;

        DockAggregate       docks;
        PierAggregate       piers;
        HabitatAggregate    habitats;
        StorageAggregate    storages;
        ProductionAggregate production;
        AllModulesAggregate modules;
        WareAggregate       wares;
        WorkforceAggregate  workforce;

        void add(types::module::Dock &&m);

        void add(types::module::Pierr &&m);

        void add(types::module::Habitat &&m);

        void add(types::module::Storage &&m);

        void add(types::module::ProductionModule &&m);

        void add(types::module::ModuleWrapper &&m);

        void add(types::Ware &&m);

        void add(types::Workforce &&m);
    };
}

#endif // X4STATIONGENERATOR__STORE_HPP
