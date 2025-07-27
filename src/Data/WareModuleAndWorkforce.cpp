//
// Created by pierre on 7/17/25.
//

#include "WareModuleAndWorkforce.hpp"

#include "spdlog/spdlog.h"

static t_modules_container _g_modules{};
static t_ware_container _g_ware{};
static t_ware_groups_container _g_groups{};
static std::map<std::string, t_production_method_id> _g_production_methods{};
std::map<std::pair<t_ware_id, t_production_method_id>, t_module_id> _g_ware_to_modules{};

/**
 * Parse multiples wares from the given module
 *
 * @param module
 */
static void parse_wares(const Module &module) {
    spdlog::debug("building data from {}", module.id);
    const auto &wares = module.production;

    for (auto &ware: wares) {
        auto &group = ware.group;

        if (!_g_ware.contains(ware.id)) {
            spdlog::debug("registered new ware {}", ware.id);
            _g_ware[ware.id] = &ware;
        }

        if (!_g_groups.contains(group.id)) {
            spdlog::debug("registered new group {}", group.id);
            _g_groups[group.id] = &group;
        }

        for (auto &production: ware.production) {
            if (!_g_production_methods.contains(production.name)) {
                spdlog::debug("registered new production method {}", production.method);
                _g_production_methods.emplace(production.name, production.method);
            }

            if (production.method == module.production_method) {
                spdlog::debug("bound module {} to combination {}, {}", module.id, ware.id, production.method);
                _g_ware_to_modules[{ware.id, production.method}] = module.id;
            }
        }
    }
}

void buildDataFrom(const std::vector<Module> &modules) {
    spdlog::info("Building data");
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

const Module *getModule(const t_ware_id &id, const t_production_method_id &production_method) {
    try {
        return getModules().at(_g_ware_to_modules[{id, production_method}]);
    } catch (const std::out_of_range &e) {
        spdlog::error("could not find module producing {} with production method {}", id, production_method);
        return nullptr;
    }
}

bool isWareProduced(const t_ware_id &id) {
    return getWares().contains(id);
}

const t_production_method_id & getProductionMethodFromName(const std::string &name) {
    return _g_production_methods[name];
}
