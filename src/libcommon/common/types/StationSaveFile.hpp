//
// Created by pierre on 7/23/26.
//

#ifndef X4STATIONGENERATOR_SRC_LIBCOMMON_COMMON_TYPES_STATIONSAVEFILE_HPP
#define X4STATIONGENERATOR_SRC_LIBCOMMON_COMMON_TYPES_STATIONSAVEFILE_HPP
#include <string>
#include <unordered_map>

#include "utils/WareTargetContainer.hpp"

namespace common::types {
    struct StationSaveFile {
        std::string                                   name;
        double                                        sun;
        bool                                          workforce;
        std::unordered_map<std::string, unsigned int> storages;
        std::unordered_map<std::string, unsigned int> docks;
        std::vector<utils::WareTarget>                primary_targets;
        std::vector<utils::WareTarget>                secondary_targets;
    };
}

#endif //X4STATIONGENERATOR_SRC_LIBCOMMON_COMMON_TYPES_STATIONSAVEFILE_HPP
