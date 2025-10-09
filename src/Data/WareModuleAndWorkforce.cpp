//
// Created by pierre on 7/17/25.
//

#include "WareModuleAndWorkforce.hpp"

#include <qguiapplication_platform.h>

#include "spdlog/spdlog.h"

static t_modules_container _g_modules{};
static t_ware_container _g_ware{};
static t_ware_groups_container _g_groups{};
static std::map<std::string, t_production_method_id> _g_production_methods{};
static t_ware_to_modules_map _g_ware_to_modules_map{};
std::map<std::pair<t_ware_id, t_production_method_id>, t_module_id>
    _g_ware_to_modules{};
std::map<std::string, std::vector<std::pair<t_ware_id, double>>> _g_workforce{};

/**
 * Parse multiples wares from the given module
 *
 * @param module
 */
static void parse_wares(const Module &module) {
  spdlog::debug("building data from {}", module.id);
  const auto &wares = module.production;

  for (auto &ware : wares) {
    auto &group = ware.group;

    if (!_g_ware.contains(ware.id)) {
      spdlog::debug("registered new ware {}", ware.id);
      _g_ware[ware.id] = &ware;
    }

    if (!_g_groups.contains(group.id)) {
      spdlog::debug("registered new group {}", group.id);
      _g_groups[group.id] = &group;
    }

    for (auto &production : ware.production) {
      // Register production method if not already done
      if (!_g_production_methods.contains(production.name)) {
        spdlog::debug("registered new production method {}", production.method);
        _g_production_methods.emplace(production.name, production.method);
      }

      // Register module as producing the ware with the given production method
      if (production.method == module.production_method) {
        spdlog::debug("bound module {} to combination {}, {}", module.id,
                      ware.id, production.method);
        _g_ware_to_modules[{ware.id, production.method}] = module.id;
      }

      // Register module as producing the ware
      auto &modules = _g_ware_to_modules_map[ware.id];
      if (!modules.contains(module.id)) {
        spdlog::debug("registered module {} as producing ware {}", module.id,
                      ware.id);
        modules[module.id] = &module;
      }
    }
  }
}

void buildDataFrom(const std::vector<Module> &modules) {
  spdlog::info("Building data");
  _g_modules.clear();
  _g_ware.clear();
  _g_groups.clear();

  for (auto &module : modules) {
    _g_modules[module.id] = &module;
    parse_wares(module);
  }
}

void buildDataFrom(
    const std::map<std::string, std::vector<std::pair<t_ware_id, double>>>
        &workforce) {
  _g_workforce = workforce;
}

const t_modules_container &getModules() { return _g_modules; }

const t_modules_container &getModules(const t_ware_id &ware_id) {
  return _g_ware_to_modules_map.at(ware_id);
}

const t_ware_container &getWares() { return _g_ware; }

const t_ware_groups_container &getWareGroups() { return _g_groups; }

const Module *getModule(const t_ware_id &id,
                        const t_production_method_id &production_method) {
  try {
    return getModules().at(_g_ware_to_modules[{id, production_method}]);
  } catch (const std::out_of_range &e) {
    spdlog::error(
        "could not find module producing {} with production method {}", id,
        production_method);
    return nullptr;
  }
}

bool isWareProduced(const t_ware_id &id) { return getWares().contains(id); }

const t_production_method_id &
getProductionMethodFromName(const std::string &name) {
  return _g_production_methods[name];
}

std::vector<WareAmount> getWorkforceUsage(std::string race,
                                          unsigned int workforce_amount) {
  try {
    std::vector<WareAmount> ware_amounts;
    auto pair_data = _g_workforce.at(race);

    for (const auto &item : pair_data) {
      const auto &ware = item.first;
      const auto &consumption = item.second;

      long int final_consumption = std::ceil(consumption * workforce_amount);
      ware_amounts.emplace_back(ware, final_consumption);
    }

    return ware_amounts;
  } catch (const std::out_of_range &e) {
    spdlog::error("could not find production method {}", race);
    throw;
  }
}
