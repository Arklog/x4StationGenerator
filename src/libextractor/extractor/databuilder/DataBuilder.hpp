//
// Created by pierre on 7/7/26.
//

#ifndef X4STATIONGENERATOR__DATABUILDER_HPP
#define X4STATIONGENERATOR__DATABUILDER_HPP
#include "ModuleAggregator.hpp"

namespace extractor::databuilder {
    struct DataBuilder {
        ModuleAggregator  modules;
        HabitatAggregator habitats;
        DockAggregator    docks;
        StorageAggregator storages;
    };
} // extractor

#endif //X4STATIONGENERATOR__DATABUILDER_HPP
