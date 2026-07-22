//
// Created by pierre on 7/10/26.
//

#include "ProductionModule.hpp"

namespace common::types::module {
    ProductionModule::module_production ProductionModule::getHourlyProduction(bool work, double sun) const {
        module_production result{};
        double            total_cycle{};

        std::ranges::for_each(wares_produced, [&](const auto &ware) {
            total_cycle += ware.second.time;
        });


        std::ranges::for_each(wares_produced, [&](const auto &ware) {
            auto   ware_id    = ware.first;
            auto   v          = ware.second;
            double prod_ratio = v.time / total_cycle;
            prod_ratio        *= 3600 / v.time;

            v.amount *= work ? v.work : 1.0f;
            v.amount *= v.sun != 0 ? sun : 1.0f;
            v.amount *= prod_ratio;

            result.emplace(ware_id, std::move(v));
        });

        return result;
    }

    ProductionModule::module_usage ProductionModule::getHourlyConsumption() const {
        module_usage result{};
        double       total_cycle{};

        std::ranges::for_each(wares_produced, [&](const auto &ware) {
            total_cycle += ware.second.time;
        });

        std::ranges::for_each(wares_produced, [&](const auto &ware) {
            double time_ratio = ware.second.time / total_cycle;
            time_ratio        *= 3600 / ware.second.time;

            std::ranges::for_each(ware.second.consumed, [&](const auto &ware_consumed) {
                result[ware_consumed.first] += ware_consumed.second * time_ratio;
            });
        });

        return result;
    }
}
