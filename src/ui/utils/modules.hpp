//
// Created by pierre on 2/1/25.
//

#ifndef X4STATIONGENERATOR_MODULES_HPP
#define X4STATIONGENERATOR_MODULES_HPP

#include "../../libcommon/data/Store.hpp"

#include <string>

#include "../../libcommon/stationbuilder/defines.hpp"

struct StationSize {
    int x_plus;
    int x_minus;
    int y_plus;
    int y_minus;
    int z_plus;
    int z_minus;
};

std::string genModulePlan(const common::stationbuilder::t_x4_complex &complex, const common::data::Store &store,
                          common::stationbuilder::Settings &          settings);

#endif //X4STATIONGENERATOR_MODULES_HPP
