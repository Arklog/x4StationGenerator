//
// Created by pierre on 7/10/26.
//

#ifndef X4STATIONGENERATOR__PRODUCTIONMODULE_HPP
#define X4STATIONGENERATOR__PRODUCTIONMODULE_HPP
#include <unordered_map>

#include "Module.hpp"

namespace common::types::module {
    struct ProductionModule : Module {
        using module_production = std::unordered_map<Ware::ware_id, long long>;

        Ware::ware_id     ware_produced;
        double            time;
        size_t            amount;
        module_production production;
    };
}

#endif //X4STATIONGENERATOR__PRODUCTIONMODULE_HPP
