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
        struct ProducedWare {
            using t_ware_consumed = std::unordered_map<Ware::ware_id, long long>;

            size_t          amount;   // amount of ware produced per hours
            double          time;     // time to produce amount
            double          work;     // work factor (1.0f means none)
            double          sun;      // sun factor (0.0f means none)
            t_ware_consumed consumed; // ware consumed per cycle
        };

        using module_production = std::unordered_map<Ware::ware_id, ProducedWare>;
        using module_usage      = ProducedWare::t_ware_consumed;

        rfl::Flatten<Module> module;
        double               time;               // time required to produce the ware in seconds
        size_t               required_workforce; // required module workforce for full efficiency
        module_production    wares_produced;     // list of the ware produced by the module per cycle
        std::string          production_method;  // module production method id

        module_production getHourlyProduction(bool work, double sun) const;

        module_usage getHourlyConsumption() const;
    };
}

#endif //X4STATIONGENERATOR__PRODUCTIONMODULE_HPP
