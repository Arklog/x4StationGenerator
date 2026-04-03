//
// Created by pierre on 7/17/25.
//

#ifndef WARESANDMODULES_HPP
#define WARESANDMODULES_HPP

#include "Store.hpp"

#include <set>

#include "Data/Data.hpp"

struct TmpModulePtrCompare
{
    bool operator() (const Module *a, const Module *b) const
    {
	return a->name < b->name;
    }
};

typedef std::unordered_map<t_ware_group_id, const WareGroup *>
    t_ware_groups_container;
typedef std::unordered_map<t_ware_id, const Ware *, std::hash<std::string> >
    t_ware_container;
typedef std::unordered_map<t_module_id, const Module *> t_modules_container;
typedef std::set<const Module *, TmpModulePtrCompare> t_module_list;
typedef std::unordered_map<t_ware_id, t_modules_container,
			   std::hash<std::string> >
    t_ware_to_modules_map;

/**
 * Get the amount of ware consumed to employ an amount of a given race workforce
 *
 * @param race The race of the workforce
 * @param workforce_amount  The amount of workforce
 * @return
 */
std::vector<WareAmount> getWorkforceUsage (std::string race,
					   unsigned int workforce_amount,
					   const Store &store);

#endif // WARESANDMODULES_HPP
