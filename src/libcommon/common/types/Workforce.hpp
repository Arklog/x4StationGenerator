//
// Created by pierre on 7/10/26.
//

#ifndef X4STATIONGENERATOR__WORKFORCE_HPP
#define X4STATIONGENERATOR__WORKFORCE_HPP
#include <string>
#include <unordered_map>

#include "Ware.hpp"

namespace common::types {
    struct Workforce {
        using race_id               = std::string;
        using workforce_consumption = std::unordered_map<Ware::ware_id, double>;

        race_id               race;
        workforce_consumption consumption;
    };
}

#endif //X4STATIONGENERATOR__WORKFORCE_HPP
