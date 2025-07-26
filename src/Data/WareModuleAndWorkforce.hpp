//
// Created by pierre on 7/17/25.
//

#ifndef WARESANDMODULES_HPP
#define WARESANDMODULES_HPP

#include <set>

#include "Data/Data.hpp"

struct TmpModulePtrCompare {
    bool operator()(const Module *a, const Module *b) const {
        return a->name < b->name;
    }
};

typedef std::unordered_map<t_ware_group_id, const WareGroup *> t_ware_groups_container;
typedef std::unordered_map<t_ware_id, const Ware *> t_ware_container;
typedef std::unordered_map<t_module_id, const Module *> t_modules_container;
typedef std::set<const Module *, TmpModulePtrCompare> t_module_list;

struct Workforce {

};

void buildDataFrom(const std::vector<Module> &modules);

const t_modules_container &getModules();

const t_ware_container &getWares();

const t_ware_groups_container &getWareGroups();

/**
 * Access the module producing the ware identified by id using the production method identified by production_method
 *
 * @param id The id of the ware produced by the module
 * @param production_method The id of the production method used by the module
 * @return
 */
const Module *getModule(const t_ware_id &id, const t_production_method_id &production_method);

/**
 * Check if a ware identified by id is produced by a module
 *
 * @param id The ware id to check
 * @return true if the ware identified by id is produced by a module, false else
 */
bool isWareProduced(const t_ware_id &id);

const t_production_method_id &getProductionMethodFromName(const std::string &name);

#endif //WARESANDMODULES_HPP
