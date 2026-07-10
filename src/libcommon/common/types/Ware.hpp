//
// Created by pierre on 7/10/26.
//

#ifndef X4STATIONGENERATOR__WARE_HPP
#define X4STATIONGENERATOR__WARE_HPP
#include <string>

#include "common/types/Price.hpp"

namespace common::types {
    struct Ware {
        using ware_id       = std::string;
        using ware_group_id = std::string;

        ware_id       id;
        ware_group_id group;
        size_t        tier;
        Price         price;
    };
} // common

#endif //X4STATIONGENERATOR__WARE_HPP
