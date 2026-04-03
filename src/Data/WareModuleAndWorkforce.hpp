//
// Created by pierre on 7/17/25.
//

#ifndef WARESANDMODULES_HPP
#define WARESANDMODULES_HPP

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

void buildDataFrom (const std::vector<Module> &modules);

void buildDataFrom (
    const std::map<std::string, std::vector<std::pair<t_ware_id, double> > >
	&workforce);

/**
 * Get all ware groups
 *
 * @return A map of ware group id to ware group pointer
 */
const t_ware_groups_container &getWareGroups ();

/**
 * Check if a ware identified by id is produced by a module
 *
 * @param id The ware id to check
 * @return true if the ware identified by id is produced by a module, false else
 */
bool isWareProduced (const t_ware_id &id);

/**
 * Get the production method id from its name
 *
 * @param name The name of the production method
 * @return The id of the production method
 */
const t_production_method_id &
getProductionMethodFromName (const std::string &name);

/**
 * Return the module id from its name
 *
 * @param name
 * @return The module id, raise std::out_of_range exception if no match
 */
const t_module_id &getModuleIdFromName (const std::string &name);

/**
 * Get the amount of ware consumed to employ an amount of a given race workforce
 *
 * @param race The race of the workforce
 * @param workforce_amount  The amount of workforce
 * @return
 */
std::vector<WareAmount> getWorkforceUsage (std::string race,
					   unsigned int workforce_amount);

#endif // WARESANDMODULES_HPP
