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
        std::string   name;
        ware_group_id group;
        std::string   group_name;
        size_t        tier;
        Price         price;
        bool          produced;
    };

    struct WareAmount {
        Ware::ware_id id;
        long int      amount;
    };
} // common

#endif //X4STATIONGENERATOR__WARE_HPP
