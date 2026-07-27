//
// Created by pierre on 7/27/26.
//
#include "stationbuilder/Complex.hpp"

#include <algorithm>

namespace {
    using WareTarget = common::stationbuilder::WareTarget;

    bool targetsEqual(const WareTarget &lhs, const WareTarget &rhs) {
        return lhs.ware_id == rhs.ware_id
               && lhs.source_module == rhs.source_module
               && lhs.prodution == rhs.prodution
               && lhs.is_secondary == rhs.is_secondary;
    }

    bool targetListsEqual(const std::vector<WareTarget *> &lhs,
                          const std::vector<WareTarget *> &rhs) {
        return std::ranges::equal(lhs, rhs, [](const WareTarget *lhs_target,
                                              const WareTarget *rhs_target) {
            return targetsEqual(*lhs_target, *rhs_target);
        });
    }

    bool wareContainersEqual(
        const common::utils::WareTargetContainer &lhs,
        const common::utils::WareTargetContainer &rhs) {
        return std::ranges::equal(lhs.getTargets(), rhs.getTargets(),
                                  targetsEqual)
               && targetListsEqual(lhs.getPrimaryTargets(),
                                   rhs.getPrimaryTargets())
               && targetListsEqual(lhs.getSecondaryTargets(),
                                   rhs.getSecondaryTargets());
    }
}

bool common::stationbuilder::Complex::operator==(const Complex &other) const {
    return name == other.name
           && complex == other.complex
           && wareContainersEqual(wares, other.wares)
           && habitat_id == other.habitat_id
           && workforce == other.workforce
           && workforce_max == other.workforce_max
           && sun == other.sun;
}
