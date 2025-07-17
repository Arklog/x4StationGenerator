//
// Created by pierre on 7/17/25.
//

#ifndef WARESANDMODULES_HPP
#define WARESANDMODULES_HPP

#include "Data/Data.hpp"

typedef std::unordered_map<t_ware_group_id, const WareGroup *> t_ware_groups_container;
typedef std::unordered_map<t_ware_id, const Ware *> t_ware_container;
typedef std::unordered_map<t_module_id, const TmpModule *> t_modules_container;

void buildDataFrom(const std::vector<TmpModule> &modules);

const t_modules_container &getModules();

const t_ware_container &getWares();

const t_ware_groups_container &getWareGroups();

#endif //WARESANDMODULES_HPP
