//
// Created by pierre on 10/11/25.
//

#include "WareTargetContainer.hpp"

#include "Data/WareModuleAndWorkforce.hpp"

WareTargetContainer::WareTargetContainer()
    : ware_targets{}, ware_targets_primary{}, ware_targets_secondary{} {
    auto wares = getWares();
    ware_targets.reserve(wares.size());

    for (const auto &[ware_id, ware]: wares) {
        const auto &modules = getModules(ware_id);

        ware_targets.emplace_back(ware_id, modules.begin()->second->id);
    }
}

bool WareTargetContainer::isPrimaryTarget(
    const t_ware_id &ware_id,
    ware_targets_container_t::const_iterator *iter) const {
    const auto check = std::ranges::find_if(ware_targets_primary,
                                            [ware_id](const WareTarget *target) {
                                                return target->ware_id == ware_id;
                                            });
    if (iter)
        *iter = check;

    return check != ware_targets_primary.end();
}

bool WareTargetContainer::isSecondaryTarget(
    const t_ware_id &ware_id,
    ware_targets_container_t::const_iterator *iter) const {
    const auto check = std::ranges::find_if(ware_targets_secondary,
                                            [ware_id](const WareTarget *target) {
                                                return target->ware_id == ware_id;
                                            });

    if (iter)
        *iter = check;

    return check != ware_targets_secondary.end();
}

void WareTargetContainer::setPrimaryTarget(const t_ware_id &ware_id) {
    if (isPrimaryTarget(ware_id))
        return;

    // Check if ware is a secondary target, if so remove it from secondary targets
    ware_targets_container_t::const_iterator iter;
    if (isSecondaryTarget(ware_id, &iter))
        ware_targets_secondary.erase(iter);

    // Find the ware in the main list and add it to primary targets
    auto ware_iter =
            std::ranges::find_if(ware_targets, [ware_id](const WareTarget &target) {
                return target.ware_id == ware_id;
            });
    if (ware_iter == ware_targets.end())
        throw std::logic_error("Ware not found in ware targets");

    ware_targets_primary.push_back(&*ware_iter);
}

void WareTargetContainer::unsetPrimaryTarget(const t_ware_id &ware_id) {
    ware_targets_container_t::const_iterator iter;

    if (!isPrimaryTarget(ware_id, &iter))
        throw std::out_of_range("Ware is not a primary target");

    (*iter)->prodution = 0;
    ware_targets_primary.erase(iter);
}

void WareTargetContainer::setSecondaryTarget(const t_ware_id ware_id,
                                             bool allow_primary_switch) {
    ware_targets_container_t::const_iterator iter;
    if (isSecondaryTarget(ware_id, &iter))
        return;

    if (isPrimaryTarget(ware_id, &iter)) {
        if (!allow_primary_switch)
            throw std::logic_error("Ware is primary target, cannot set as secondary");
        ware_targets_secondary.push_back(*iter);
        ware_targets_primary.erase(iter);
        return;
    }

    ware_targets_secondary.push_back(
        const_cast<WareTarget *>(getTarget(ware_id)));
}

WareTarget *
WareTargetContainer::getPrimaryTarget(const t_ware_id &ware_id) const {
    ware_targets_container_t::const_iterator iter;
    if (!isPrimaryTarget(ware_id, &iter))
        throw std::out_of_range("Ware is not a primary target");

    return *iter;
}

WareTarget *
WareTargetContainer::getSecondaryTarget(const t_ware_id &ware_id) const {
    ware_targets_container_t::const_iterator iter;
    if (!isSecondaryTarget(ware_id, &iter))
        throw std::out_of_range("Ware is not a secondary target");

    return *iter;
}

WareTarget *WareTargetContainer::getTarget(const t_ware_id &ware_id) const {
    const auto iter =
            std::ranges::find_if(ware_targets, [ware_id](const WareTarget &target) {
                return target.ware_id == ware_id;
            });

    if (iter == ware_targets.end())
        throw std::out_of_range("Ware is not a target");

    return const_cast<WareTarget *>(&*iter);
}

const std::vector<WareTarget *> WareTargetContainer::getPrimaryAndSecondaryTargets() const {
    ware_targets_container_t all_targets{};
    all_targets.reserve(ware_targets_primary.size() + ware_targets_secondary.size());

    all_targets.insert(all_targets.end(), ware_targets_primary.begin(), ware_targets_primary.end());
    all_targets.insert(all_targets.end(), ware_targets_secondary.begin(), ware_targets_secondary.end());

    return all_targets;
}

const std::vector<WareTarget *> &WareTargetContainer::getPrimaryTargets() const {
    return ware_targets_primary;
}

const std::vector<WareTarget *> &WareTargetContainer::getSecondaryTargets() const {
    return ware_targets_secondary;
}
