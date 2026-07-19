//
// Created by pierre on 7/10/26.
//

#include "Workforce.hpp"

#include <algorithm>
#include <cmath>

namespace common::types {
    Workforce::workforce_consumption Workforce::getConsumption(unsigned int amount) const {
        workforce_consumption consumption{};

        std::ranges::for_each(this->consumption, [&consumption, amount](auto consumed_ware) {
            consumed_ware.second = std::ceil(consumed_ware.second * amount);
            consumption.emplace(consumed_ware.first, consumed_ware.second);
        });

        return consumption;
    }
}
