//
// Created by pierre on 10/11/25.
//

#ifndef X4STATIONGENERATOR_WARETARGETCONTAINER_H
#define X4STATIONGENERATOR_WARETARGETCONTAINER_H
#include "StationBuilder/defines.hpp"

class WareTargetContainer {
private:
    using ware_targets_main_container_t = std::vector<WareTarget>;
    using ware_targets_container_t = std::vector<WareTarget *>;
    // using ware_targets_container_iter_t =
    // std::ranges::borrowed_iterator_t<ware_targets_container_t>;

    ware_targets_main_container_t ware_targets;
    ware_targets_container_t ware_targets_primary;
    ware_targets_container_t ware_targets_secondary;

public:
    WareTargetContainer();

    /**
     * Check if a ware is a primary target
     * @param ware_id
     * @param iter
     * @return
     */
    [[nodiscard]] bool isPrimaryTarget(
        const t_ware_id &ware_id,
        ware_targets_container_t::const_iterator *iter = nullptr) const;

    /**
     * Check if a ware is a secondary target
     * @param ware_id
     * @param iter
     * @return
     */
    bool isSecondaryTarget(
        const t_ware_id &ware_id,
        ware_targets_container_t::const_iterator *iter = nullptr) const;

    /**
     * Set a ware as primary target
     * @return
     */
    void setPrimaryTarget(const t_ware_id &ware_id);

    /**
     * Unset a ware as primary target, if the ware is not a primary target throw
     * std::out_of_range
     *
     * @param ware_id
     */
    void unsetPrimaryTarget(const t_ware_id &ware_id);

    /**
     * Set ware as secondary target, if the ware is already a primary target and
     * allow_primary_switch is false std::logic_error is thrown.
     *
     * @param ware_id
     * @param allow_primary_switch
     */
    void setSecondaryTarget(t_ware_id ware_id, bool allow_primary_switch = false);

    /**
     * Get a primary target by ware id, if the ware is not a primary target throw
     * std::out_of_range
     *
     * @param ware_id
     * @return
     */
    [[nodiscard]] WareTarget *getPrimaryTarget(const t_ware_id &ware_id) const;

    /**
     * Get a secondary target by ware id, if the ware is not a secondary target
     * throw std::out_of_range
     *
     * @param ware_id
     * @return
     */
    [[nodiscard]] WareTarget *getSecondaryTarget(const t_ware_id &ware_id) const;

    /**
     * Get a target by ware id, if the ware is not a target throw
     * std::out_of_range
     *   * @param ware_id
     * @return
     */
    [[nodiscard]] WareTarget *getTarget(const t_ware_id &ware_id) const;

    [[nodiscard]] const std::vector<WareTarget *> &getPrimaryTargets() const;

    [[nodiscard]] const std::vector<WareTarget *> &getSecondaryTargets() const;

    [[nodiscard]] const std::vector<WareTarget *> getPrimaryAndSecondaryTargets() const;
};

#endif // X4STATIONGENERATOR_WARETARGETCONTAINER_H
