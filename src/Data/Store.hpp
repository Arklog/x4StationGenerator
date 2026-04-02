//
// Created by pierre on 3/31/26.
//

#ifndef X4STATIONGENERATOR__STORE_HPP
#define X4STATIONGENERATOR__STORE_HPP
#include "Data.hpp"

#include <list>

struct Store
{
    using module_container_type = std::list<Module>;
    using ware_container_type = std::list<Ware>;
    using ware_group_container_type = std::list<WareGroup>;

    struct {
        module_container_type all; // Contain all modules
	std::vector<Module *> production; // All production modules
	std::vector<Module *> habitats; // All habitats modules
        std::vector<Module *> docks_and_piers; // All dock or pier modules
        std::vector<Module *> storages; // All storage modules
	std::unordered_map<t_module_id, Module*> by_id; // Reference modules by id
        std::unordered_map<std::string, Module*> by_name; // Reference modules by module name
        std::unordered_map<t_ware_id, std::vector<Module*>> producing; // Reference module by produced ware
    } modules;

    struct
    {
        ware_container_type all;
        std::unordered_map<t_ware_id, Ware*> by_id;
        std::unordered_map<std::string, Ware*> by_name;
    } wares;

    struct
    {
        ware_group_container_type all;
        std::unordered_map<t_ware_group_id, WareGroup *> by_id;
        std::unordered_map<std::string, WareGroup *> by_name;
    } ware_groups;

    /**
     * Register a new module.
     *
     * @param module
     */
    void registerModule(Module &&module);
};

#endif //X4STATIONGENERATOR__STORE_HPP
