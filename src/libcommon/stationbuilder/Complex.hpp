//
// Created by pierre on 7/21/26.
//

#ifndef X4STATIONGENERATOR_SRC_LIBCOMMON_STATIONBUILDER_COMPLEX_HPP
#define X4STATIONGENERATOR_SRC_LIBCOMMON_STATIONBUILDER_COMPLEX_HPP
#include "defines.hpp"
#include "utils/WareTargetContainer.hpp"

namespace common::stationbuilder {
    struct Complex {
        std::string                name;
        t_x4_complex               complex;
        utils::WareTargetContainer wares;
        size_t                     workforce;
        size_t                     workforce_max;
        double                     sun;
    };
}

#endif //X4STATIONGENERATOR_SRC_LIBCOMMON_STATIONBUILDER_COMPLEX_HPP
