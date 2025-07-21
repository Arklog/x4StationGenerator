//
// Created by pierre on 2/1/25.
//

#ifndef X4STATIONGENERATOR_MODULES_HPP
#define X4STATIONGENERATOR_MODULES_HPP

#include <string>

#include "StationBuilder/defines.hpp"

struct StationSize {
    int x_plus;
    int x_minus;
    int y_plus;
    int y_minus;
    int z_plus;
    int z_minus;
};

std::string genModulePlan(const std::string &plan_name, const t_x4_complex &complex);

#endif //X4STATIONGENERATOR_MODULES_HPP
