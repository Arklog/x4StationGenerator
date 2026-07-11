//
// Created by pierre on 7/10/26.
//

#ifndef X4STATIONGENERATOR__PRODUCTIONMODULE_HPP
#define X4STATIONGENERATOR__PRODUCTIONMODULE_HPP
#include <unordered_map>
#include <vector>

#include <rfl/cli.hpp>
#include "Module.hpp"

namespace common::types::module {
    struct ProductionModule {
        using module_production      = std::unordered_map<Ware::ware_id, unsigned int>;
        using module_usage           = std::unordered_map<Ware::ware_id, signed int>;
        using module_ware_derivative = std::unordered_map<Ware::ware_id, long long>;

        rfl::Flatten<Module>   module;
        double                 time;               // time required to produce the ware in seconds
        size_t                 required_workforce; // required module workforce for full efficiency
        module_production      wares_produced;     // list of the ware produced by the module per cycle
        module_usage           wares_required;     // list of the ware used by the module per cycle
        module_ware_derivative wares_derivative;   // list of ware produced and used by the module per cycle
        std::string            production_method;  // module production method id
    };
}

#endif //X4STATIONGENERATOR__PRODUCTIONMODULE_HPP
