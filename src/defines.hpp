//
// Created by pierre on 2/9/25.
//

#ifndef X4STATIONGENERATOR_DEFINES_HPP
#define X4STATIONGENERATOR_DEFINES_HPP

#include <string>
#include <map>
#include <vector>

enum class RESSOURCE {
    METHANE,
    HYDROGEN,
    HELIUM,
    ORE,
    SILICON,
    NIVIDIUM,
    ICE,
    ENERGY_CELL,
    GRAPHENE,
    REFINED_METAL,
    SILICON_WAFER,
    SUPERFLUID_COOLANT,
    HULL_PART,
    WATER,
    SPICES,
    WHEAT,
    FOOD_RATIONS,
    MEDICAL_SUPPLIES,
    MEAT,
    CLAYTRONIC,
    ANTIMATTER_CONVERTER,
    ANTIMATTER_CELL,
    MICROCHIP,
    QUANTUM_TUBE,
    DRONE_COMPONENTS,
    ENGINE_PARTS,
    SCANNING_ARRAY,
    SHIELD_COMPONENTS,
    PLASMA_CONDUCTOR,
    WEAPON_COMPONENTS,
    SMARTCHIPS,
    TURRET_COMPONENTS,
};

typedef enum MODULE_TYPE_ENUM {
    DOCK              = 1,
    PIER              = DOCK << 1,
    STORAGE           = PIER << 1,
    HABITAT           = STORAGE << 1,
    PRODUCTION        = HABITAT << 1,
    DOCK_OR_PIER      = DOCK | PIER,
    DOCK_PIER_STORAGE = DOCK | PIER | STORAGE,
} MODULE_TYPE;

struct StationSize {
    int x_plus;
    int x_minus;
    int y_plus;
    int y_minus;
    int z_plus;
    int z_minus;
};

typedef std::map<RESSOURCE, int> t_ressources;
const t_ressources               WORKFORCE_CONSUMPTION_PER_50 = {
        {RESSOURCE::FOOD_RATIONS,     113},
        {RESSOURCE::MEDICAL_SUPPLIES, 68}
};

struct Module {
    std::string  name;
    std::string  macro;
    MODULE_TYPE  type;
    t_ressources ressources_produced;
    t_ressources ressources_produced_max;
    t_ressources ressources_consumed;
    t_ressources build_cost;
    int          workforce_max;

    [[nodiscard]] t_ressources getTotal(int nmodules = 1, bool workforce_is_max = false) const;
};

typedef std::map<Module, int> t_modules;
typedef std::vector<Module>   t_module_list;

#endif //X4STATIONGENERATOR_DEFINES_HPP
