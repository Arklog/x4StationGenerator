//
// Created by pierre on 7/17/25.
//

#include "WaresAndModules.hpp"

static t_modules_container _g_modules{};
static t_ware_container _g_ware{};
static t_ware_groups_container _g_groups{};

static void parse_wares(const std::vector<Ware> &wares) {
    for (auto& ware: wares) {
         auto& group = ware.group;

        if (!_g_ware.contains(ware.id))
            _g_ware[ware.id] = &ware;

        if (!_g_groups.contains(group.id))
            _g_groups[group.id] = &group;
    }
}

void buildDataFrom(const std::vector<TmpModule> &modules) {
    _g_modules.clear();
    _g_ware.clear();
    _g_groups.clear();

    for (auto& module : modules) {
        _g_modules[module.id] = &module;
        parse_wares(module.product);
    }
}

const t_modules_container & getModules() {
    return _g_modules;
}

const t_ware_container & getWares() {
    return _g_ware;
}

const t_ware_groups_container & getWareGroups() {
    return _g_groups;
}
