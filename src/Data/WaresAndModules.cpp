//
// Created by pierre on 7/17/25.
//

#include "WaresAndModules.hpp"

static t_modules_container _g_modules{};
static t_ware_container _g_ware{};
static t_ware_groups_container _g_groups{};
static std::map<std::pair<t_ware_id, t_production_method_id>, t_module_id> _g_ware_to_modules{};

/**
 * Parse multiples wares from the given module
 *
 * @param module
 */
static void parse_wares(const TmpModule &module) {
    const auto &wares = module.product;

    for (auto &ware: wares) {
        auto &group = ware.group;

        if (!_g_ware.contains(ware.id))
            _g_ware[ware.id] = &ware;

        if (!_g_groups.contains(group.id))
            _g_groups[group.id] = &group;

        for (auto &production: ware.production) {
            _g_ware_to_modules[{ware.id, production.name}] = module.id;
        }
    }
}

void buildDataFrom(const std::vector<TmpModule> &modules) {
    _g_modules.clear();
    _g_ware.clear();
    _g_groups.clear();

    for (auto &module: modules) {
        _g_modules[module.id] = &module;
        parse_wares(module);
    }
}

const t_modules_container &getModules() {
    return _g_modules;
}

const t_ware_container &getWares() {
    return _g_ware;
}

const t_ware_groups_container &getWareGroups() {
    return _g_groups;
}

const TmpModule *getModule(const t_ware_id &id, const t_production_method_id &production_method) {
    return getModules().at(_g_ware_to_modules[{id, production_method}]);
}
