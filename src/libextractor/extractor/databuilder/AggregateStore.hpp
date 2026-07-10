//
// Created by pierre on 7/7/26.
//

#ifndef X4STATIONGENERATOR__AGGREGATESTORE_HPP
#define X4STATIONGENERATOR__AGGREGATESTORE_HPP
#include "ModuleAggregator.hpp"
#include "extractor/models/model_store.hpp"

namespace extractor::databuilder {
    struct AggregateStore {
        AggregateStore(models::ModelStore &&model_store);

        ModuleAggregator     modules;
        HabitatAggregator    habitats;
        DockAggregator       docks;
        StorageAggregator    storages;
        WareAggregator       wares;
        WaregroupsAggregator waregroups;
    };
} // extractor

#endif //X4STATIONGENERATOR__AGGREGATESTORE_HPP
