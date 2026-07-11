//
// Created by pierre on 7/4/26.
//

#ifndef X4STATIONGENERATOR__MODULE_HPP
#define X4STATIONGENERATOR__MODULE_HPP
#include <string>
#include <unordered_map>

#include "common/types/Price.hpp"
#include "common/types/Ware.hpp"

namespace common::types::module {
    struct Module {
        using module_id   = std::string;
        using module_cost = std::unordered_map<Ware::ware_id, unsigned int>;

        module_id   id;
        std::string name;
        Price       price;
        module_cost cost;
    };
} // namespace common

#endif // X4STATIONGENERATOR__MODULE_HPP
