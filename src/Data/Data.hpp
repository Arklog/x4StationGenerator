//
// Created by pierre on 10/19/25.
//

#ifndef X4STATIONGENERATOR_DATA_HPP
#define X4STATIONGENERATOR_DATA_HPP
#include "RawData.hpp"

struct ModuleData {
    using t_module_map = std::unordered_map<t_module_id, const Module *>;

    std::vector<Module> modules;

    t_module_map module_map;              // map of all modules by id
    t_module_map container_map;           // map of all container modules by id
    t_module_map container_container_map; // map of all container container modules by id
    t_module_map container_solid_map;     // map of all solid container modules by id
    t_module_map container_liquid_map;    // map of all liquid container modules by id
    t_module_map production_map;          // map of all production modules by id
    t_module_map processing_map;          // map of all processing modules by id
    t_module_map dock_and_pierr_map;      // map of all dock and pierr modules by id
    t_module_map dock_map;                // map of all dock modules by id
    t_module_map pierr_map;               // map of all pierr modules by id
    t_module_map habitation_map;          // map of all habitation modules by id
    t_module_map claim_map;               // map of all claim modules by id
    t_module_map defence_map;             // map of all defence modules by id
    t_module_map build_map;               // map of all build modules by id
    t_module_map equip_map;               // map of all equip modules by id
    t_module_map connection_map;          // map of all connection modules by id
    t_module_map venture_map;             // map of all venture modules by id

    std::unordered_map<std::string, const Module *> module_name_map; // map of module pointer to name
};

struct WareData {
    std::unordered_map<t_ware_id, const Ware *>   ware_map;      // map of all wares by id
    std::unordered_map<std::string, const Ware *> ware_name_map; // map of ware name to ware pointer
};

struct WareGroupData {
    std::unordered_map<t_ware_group_id, const WareGroup *> ware_group_map; // map of all ware groups by id
};

struct RelationshipData {
    // map of ware id to list of module ids producing it
    std::unordered_map<t_ware_id, std::unordered_map<t_module_id, const Module *> > production_map;
};

struct WorkforceData {
    std::map<t_race_id, std::map<t_ware_id, double> > consumption_map;
};

class Data {
private:
    static void registerWare(const Ware &ware);

    static void registerWareGroup(const WareGroup &ware_group);

    static void registerRelationship(const Module &module, const Ware &ware);

    static void processModule(const Module &module);

public:
    Data() = default;

    static std::shared_ptr<ModuleData>       modules;
    static std::shared_ptr<WareData>         wares;
    static std::shared_ptr<WareGroupData>    ware_groups;
    static std::shared_ptr<RelationshipData> relationships;
    static std::shared_ptr<WorkforceData>    workforce;

    /**
     * Register a module and build related data
     * @param module
     */
    static void registerModule(const Module &module);

    /**
     * Register workforce consumption data
     * @param workforce
     */
    static void registerWorkforce(
        const std::map<t_race_id, std::map<t_ware_id, double> > &workforce);

    /**
     * Build data
     */
    static void processData();

    /**
     * Check if the ware identified by ware is produced by any module
     * @param ware
     * @return
     */
    static bool isWareProduced(const t_ware_id &ware);

    /**
     * Get ressource consumption from workforce of a given race
     * @param race the race of the workforce
     * @param workforce_amount the amount of workforce
     * @return a vector of ware amounts
     */
    static std::vector<WareAmount> getWorkforceUsage(t_race_id race, unsigned int workforce_amount);
};

#endif // X4STATIONGENERATOR_DATA_HPP
