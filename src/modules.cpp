//
// Created by pierre on 2/1/25.
//
#include "modules.hpp"

std::map<RESSOURCE, int> Module::getTotal(int nmodules, bool workforce_is_max, size_t sun) const
{
    std::map<RESSOURCE, int>       tmp;
    const std::map<RESSOURCE, int> &r = workforce_is_max ? this->ressources_produced_max : this->ressources_produced;

    for (auto i: r) {
        int v = i.second * nmodules;
        if (i.first == RESSOURCE::ENERGY_CELL && this->sun_factor)
            v = static_cast<int>(static_cast<double>(v) * (static_cast<double>(sun) / 100));
        tmp[i.first] = v;
    }
    for (auto i: ressources_consumed) {
        tmp[i.first] = -i.second * nmodules;
    }

    return tmp;
}

bool operator<(const Module &a, const Module &b)
{
    const char *ca = a.name.c_str();
    const char *cb = b.name.c_str();

    return std::strcmp(ca, cb) < 0;
}

namespace MODULES {
    const Module ARGON_L_STORAGE_CONTAINER = {
            "Argon L Storage Container",
            "storage_arg_l_container_01_macro",
            MODULE_TYPE::STORAGE,
            {},
            {},
            {},
            {
                    {RESSOURCE::CLAYTRONIC, 115},
                    {RESSOURCE::HULL_PART, 421},
                    {RESSOURCE::ENERGY_CELL, 115}
            },
            0,
    };

    const Module ARGON_L_STORAGE_SOLID = {
            "Argon L Storage Solid",
            "storage_arg_l_solid_01_macro",
            MODULE_TYPE::STORAGE,
            {},
            {},
            {},
            {
                    {RESSOURCE::CLAYTRONIC, 115},
                    {RESSOURCE::HULL_PART, 421},
                    {RESSOURCE::ENERGY_CELL, 115}
            },
            0,
    };

    const Module ARGON_L_STORAGE_LIQUID = {
            "Argon L Storage Liquid",
            "storage_arg_l_liquid_01_macro",
            MODULE_TYPE::STORAGE,
            {},
            {},
            {},
            {
                    {RESSOURCE::CLAYTRONIC, 115},
                    {RESSOURCE::HULL_PART, 421},
                    {RESSOURCE::ENERGY_CELL, 115}
            },
            0,
    };

    const Module ARGON_PIER_1 = {
            "Argon 1-Dock Pier",
            "pier_arg_harbor_02_macro",
            MODULE_TYPE::PIER,
            {},
            {},
            {},
            {
                    {RESSOURCE::CLAYTRONIC, 313},
                    {RESSOURCE::ENERGY_CELL, 625},
                    {RESSOURCE::HULL_PART, 1143},
            },
            0,
    };

    const Module ARGON_BASIC_DOCK_3M_6S = {
            "Argon Basic Dock 3M 6S",
            "dockarea_arg_m_station_02_lowtech_macro",
            MODULE_TYPE::DOCK,
            {},
            {},
            {},
            {
                    {RESSOURCE::CLAYTRONIC, 75},
                    {RESSOURCE::ENERGY_CELL, 150},
                    {RESSOURCE::HULL_PART, 274},
            },
            0,
    };

    const Module ARGON_L_HABITAT = {
            "Argon L Habitat",
            "hab_arg_l_01_macro",
            MODULE_TYPE::HABITAT,
            {},
            {},
            {},
            {
                    {RESSOURCE::CLAYTRONIC, 191},
                    {RESSOURCE::ENERGY_CELL, 383},
                    {RESSOURCE::HULL_PART, 700},
            },
            0,
    };

    const Module SOLAR_POWER_PLANT = {
            "Energy Cell Production",
            "prod_gen_energycells_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::ENERGY_CELL, 10500}},
            {{RESSOURCE::ENERGY_CELL, 15015}},
            {},
            {
                    {RESSOURCE::CLAYTRONIC, 260},
                    {RESSOURCE::ENERGY_CELL, 520},
                    {RESSOURCE::HULL_PART, 951},
            },
            90,
            true
    };

    const Module WATER_PRODUCTION = {
            "Water Production",
            "prod_gen_water_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::WATER, 5790}},
            {{RESSOURCE::WATER, 8280}},
            {
                    {RESSOURCE::ICE, 9600},
                    {RESSOURCE::ENERGY_CELL, 1800}
            },
            {
                    {RESSOURCE::CLAYTRONIC, 36},
                    {RESSOURCE::ENERGY_CELL, 72},
                    {RESSOURCE::HULL_PART, 132},
            },
            180
    };

    const Module MEAT_PRODUCTION = {
            "Meat Production",
            "prod_arg_meat_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::MEAT, 2320}},
            {{RESSOURCE::MEAT, 3086}},
            {
                    {RESSOURCE::WATER, 800},
                    {RESSOURCE::ENERGY_CELL, 640}
            },
            {
                    {RESSOURCE::CLAYTRONIC, 248},
                    {RESSOURCE::ENERGY_CELL, 497},
                    {RESSOURCE::HULL_PART, 910},
            },
            75,
    };

    const Module SPICE_PRODUCTION = {
            "Spice Production",
            "prod_gen_spices_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::SPICES, 3000}},
            {{RESSOURCE::SPICES, 4200}},
            {
                    {RESSOURCE::WATER, 480},
                    {RESSOURCE::ENERGY_CELL, 240}
            },
            {
                    {RESSOURCE::CLAYTRONIC, 139},
                    {RESSOURCE::ENERGY_CELL, 278},
                    {RESSOURCE::HULL_PART, 510},
            },
            60,
    };

    const Module WHEAT_PRODUCTION = {
            "Wheat Production",
            "prod_arg_wheat_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::WHEAT, 3720}},
            {{RESSOURCE::WHEAT, 4762}},
            {
                    {RESSOURCE::WATER, 960},
                    {RESSOURCE::ENERGY_CELL, 720}
            },
            {
                    {RESSOURCE::CLAYTRONIC, 296},
                    {RESSOURCE::ENERGY_CELL, 592},
                    {RESSOURCE::HULL_PART, 1084},
            },
            75
    };

    const Module FOOD_RATION_PRODUCTION = {
            "Food Ration Production",
            "prod_arg_foodrations_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::FOOD_RATIONS, 6900}},
            {{RESSOURCE::FOOD_RATIONS, 9113 + 203}},
            {
                    {RESSOURCE::WHEAT, 600},
                    {RESSOURCE::MEAT, 600},
                    {RESSOURCE::SPICES, 600},
                    {RESSOURCE::ENERGY_CELL, 1500}
            },
            {
                    {RESSOURCE::CLAYTRONIC, 262},
                    {RESSOURCE::ENERGY_CELL, 525},
                    {RESSOURCE::HULL_PART, 961},
            },
            90
    };

    const Module ARGON_MEDICAL_SUPPLY_PRODUCTION = {
            "Argon Medical Supply Production",
            "prod_arg_medicalsupplies_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::MEDICAL_SUPPLIES, 2496}},
            {{RESSOURCE::MEDICAL_SUPPLIES, 3073 + 122}},
            {
                    {RESSOURCE::ENERGY_CELL, 1200},
                    {RESSOURCE::WATER, 720},
                    {RESSOURCE::SPICES, 480},
                    {RESSOURCE::WHEAT, 360},
            },
            {
                    {RESSOURCE::CLAYTRONIC, 225},
                    {RESSOURCE::ENERGY_CELL, 450},
                    {RESSOURCE::HULL_PART, 823},
            },
            90,
    };

    const Module GRAPHENE_PRODUCTION = {
            "Graphene Production",
            "prod_gen_graphene_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::GRAPHENE, 1440}},
            {{RESSOURCE::GRAPHENE, 2102}},
            {
                    {RESSOURCE::METHANE, 4800},
                    {RESSOURCE::ENERGY_CELL, 1200}
            },
            {
                    {RESSOURCE::CLAYTRONIC, 28},
                    {RESSOURCE::ENERGY_CELL, 57},
                    {RESSOURCE::HULL_PART, 104},
            },
            180,
    };

    const Module REFINED_METHAL_PRODUCTION = {
            "Refined Metals Production",
            "prod_gen_refinedmetals_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::REFINED_METAL, 2112}},
            {{RESSOURCE::REFINED_METAL, 3020}},
            {
                    {RESSOURCE::ORE, 5760},
                    {RESSOURCE::ENERGY_CELL, 2160}
            },
            {
                    {RESSOURCE::CLAYTRONIC, 36},
                    {RESSOURCE::ENERGY_CELL, 73},
                    {RESSOURCE::HULL_PART, 135},
            },
            225,
    };

    const Module HULL_PART_FACTORY = {
            "Hull Part Factory",
            "prod_gen_hullparts_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::HULL_PART, 1176}},
            {{RESSOURCE::HULL_PART, 1611}},
            {
                    {RESSOURCE::ENERGY_CELL, 320},
                    {RESSOURCE::GRAPHENE, 160},
                    {RESSOURCE::REFINED_METAL, 1120},
            },
            {
                    {RESSOURCE::CLAYTRONIC, 614},
                    {RESSOURCE::ENERGY_CELL, 1229},
                    {RESSOURCE::HULL_PART, 2249},
            },
            270,
    };

    const Module CLAYTRONICS_PRODUCTION = {
            "Claytronics Production",
            "prod_gen_claytronics_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::CLAYTRONIC, 432}},
            {{RESSOURCE::CLAYTRONIC, 579}},
            {
                    {RESSOURCE::ENERGY_CELL, 560},
                    {RESSOURCE::ANTIMATTER_CELL, 400},
                    {RESSOURCE::MICROCHIP, 640},
                    {RESSOURCE::QUANTUM_TUBE, 400},
            },
            {
                    {RESSOURCE::CLAYTRONIC, 2822},
                    {RESSOURCE::ENERGY_CELL, 5642},
                    {RESSOURCE::HULL_PART, 10327}
            },
            1215
    };

    const Module ANTIMATTER_CELL_PRODUCTION = {
            "Antimatter Cell Production",
            "prod_gen_antimattercells_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::ANTIMATTER_CELL, 2970}},
            {{RESSOURCE::ANTIMATTER_CELL, 4010}},
            {
                    {RESSOURCE::HYDROGEN, 9600},
                    {RESSOURCE::ENERGY_CELL, 3000},
            },
            {
                    {RESSOURCE::CLAYTRONIC, 69},
                    {RESSOURCE::ENERGY_CELL, 138},
                    {RESSOURCE::HULL_PART, 253}
            },
            180
    };

    const Module MICROCHIP_PRODUCTION = {
            "Microchip Production",
            "prod_gen_microchips_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::MICROCHIP, 432}},
            {{RESSOURCE::MICROCHIP, 588}},
            {
                    {RESSOURCE::ENERGY_CELL, 300},
                    {RESSOURCE::SILICON_WAFER, 1200},
            },
            {
                    {RESSOURCE::CLAYTRONIC, 758},
                    {RESSOURCE::ENERGY_CELL, 1516},
                    {RESSOURCE::HULL_PART, 2774},
            },
            450,
    };

    const Module QUANTUM_TUBE_PRODUCTION = {
            "Quantum Tube Production",
            "prod_gen_quantumtubes_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::QUANTUM_TUBE, 470}},
            {{RESSOURCE::QUANTUM_TUBE, 719}},
            {
                    {RESSOURCE::ENERGY_CELL, 200},
                    {RESSOURCE::GRAPHENE, 580},
                    {RESSOURCE::SUPERFLUID_COOLANT, 150},
            },
            {
                    {RESSOURCE::CLAYTRONIC, 380},
                    {RESSOURCE::ENERGY_CELL, 761},
                    {RESSOURCE::HULL_PART, 1394},
            },
            225,
    };

    const Module SILICON_WAFER_PRODUCTION = {
            "Silicon Wafer Production",
            "prod_gen_siliconwafers_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::SILICON_WAFER, 2140}},
            {{RESSOURCE::SILICON_WAFER, 2932}},
            {
                    {RESSOURCE::ENERGY_CELL, 1800},
                    {RESSOURCE::SILICON, 4800}
            },
            {
                    {RESSOURCE::CLAYTRONIC, 74},
                    {RESSOURCE::ENERGY_CELL, 149},
                    {RESSOURCE::HULL_PART, 273}
            },
            180
    };

    const Module SUPERFLUID_COOLANT_PRODUCTION = {
            "Superfluid Coolant Production",
            "prod_gen_superfluidcoolant_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::SUPERFLUID_COOLANT, 1425}},
            {{RESSOURCE::SUPERFLUID_COOLANT, 2123}},
            {
                    {RESSOURCE::ENERGY_CELL, 900},
                    {RESSOURCE::HELIUM, 4800},
            },
            {
                    {RESSOURCE::CLAYTRONIC, 25},
                    {RESSOURCE::ENERGY_CELL, 51},
                    {RESSOURCE::HULL_PART, 94},
            },
            225
    };

    const Module DRONE_COMPONENTS_PRODUCTION = {
            "Drone Components Production",
            "prod_gen_dronecomponents_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::DRONE_COMPONENTS, 315}},
            {{RESSOURCE::DRONE_COMPONENTS, 450}},
            {
                    {RESSOURCE::ENERGY_CELL, 180},
                    {RESSOURCE::ENGINE_PARTS, 60},
                    {RESSOURCE::HULL_PART, 60},
                    {RESSOURCE::MICROCHIP, 60},
                    {RESSOURCE::SCANNING_ARRAY, 120}
            },
            {
                    {RESSOURCE::CLAYTRONIC, 1225},
                    {RESSOURCE::ENERGY_CELL, 2449},
                    {RESSOURCE::HULL_PART, 4483},
            },
            675
    };

    const Module ENGINE_PARTS_PRODUCTION = {
            "Engine Parts Production",
            "prod_gen_engineparts_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::ENGINE_PARTS, 832}},
            {{RESSOURCE::ENGINE_PARTS, 1223}},
            {
                    {RESSOURCE::ENERGY_CELL, 240},
                    {RESSOURCE::ANTIMATTER_CELL, 320},
                    {RESSOURCE::REFINED_METAL, 384}
            },
            {
                    {RESSOURCE::CLAYTRONIC, 389},
                    {RESSOURCE::ENERGY_CELL, 779},
                    {RESSOURCE::HULL_PART, 1426},
            },
            225
    };

    const Module SHIELD_COMPONENTS_PRODUCTION = {
            "Shield Components Production",
            "prod_gen_shieldcomponents_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::SHIELD_COMPONENTS, 579}},
            {{RESSOURCE::SHIELD_COMPONENTS, 828}},
            {
                    {RESSOURCE::ENERGY_CELL, 210},
                    {RESSOURCE::QUANTUM_TUBE, 60},
                    {RESSOURCE::PLASMA_CONDUCTOR, 120}
            },
            {
                    {RESSOURCE::CLAYTRONIC, 432},
                    {RESSOURCE::ENERGY_CELL, 865},
                    {RESSOURCE::HULL_PART, 1583},
            },
            225
    };

    const Module SCANNING_ARRAY_PRODUCTION = {
            "Scanning Array Production",
            "prod_gen_scanningarray_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::SCANNING_ARRAY, 216}},
            {{RESSOURCE::SCANNING_ARRAY, 298}},
            {
                    {RESSOURCE::ENERGY_CELL, 360},
                    {RESSOURCE::REFINED_METAL, 600},
                    {RESSOURCE::SILICON_WAFER, 360}
            },
            {
                    {RESSOURCE::CLAYTRONIC, 629},
                    {RESSOURCE::ENERGY_CELL, 1259},
                    {RESSOURCE::HULL_PART, 2305},
            },
            315
    };

    const Module WEAPON_COMPONENTS_PRODUCTION = {
            "Weapon Components Production",
            "prod_gen_weaponcomponents_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::WEAPON_COMPONENTS, 340}},
            {{RESSOURCE::WEAPON_COMPONENTS, 520}},
            {
                    {RESSOURCE::ENERGY_CELL, 120},
                    {RESSOURCE::HULL_PART, 40},
                    {RESSOURCE::PLASMA_CONDUCTOR, 60},
            },
            {
                    {RESSOURCE::CLAYTRONIC, 396},
                    {RESSOURCE::ENERGY_CELL, 793},
                    {RESSOURCE::HULL_PART, 1452},
            },
            225
    };

    const Module SMARTCHIPS_PRODUCTION = {
            "Smart Chips Production",
            "prod_gen_smartchips_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::SMARTCHIPS, 858}},
            {{RESSOURCE::SMARTCHIPS, 1175}},
            {
                    {RESSOURCE::ENERGY_CELL, 300},
                    {RESSOURCE::SILICON_WAFER, 120},
            },
            {
                    {RESSOURCE::CLAYTRONIC, 126},
                    {RESSOURCE::ENERGY_CELL, 253},
                    {RESSOURCE::HULL_PART, 464},
            },
            60
    };

    const Module TURRET_COMPONENTS_PRODUCTION = {
            "Turret Components Production",
            "prod_gen_turretcomponents_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::TURRET_COMPONENTS, 340}},
            {{RESSOURCE::TURRET_COMPONENTS, 520}},
            {
                    {RESSOURCE::ENERGY_CELL, 120},
                    {RESSOURCE::MICROCHIP, 40},
                    {RESSOURCE::QUANTUM_TUBE, 40},
                    {RESSOURCE::SCANNING_ARRAY, 20},
            },
            {
                    {RESSOURCE::CLAYTRONIC, 475},
                    {RESSOURCE::ENERGY_CELL, 951},
                    {RESSOURCE::HULL_PART, 1741},
            },
            225
    };

    const Module PLASMA_CONDUCTOR_PRODUCTION = {
            "Plasma Conductor Production",
            "prod_gen_plasmaconductors_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::PLASMA_CONDUCTOR, 176}},
            {{RESSOURCE::PLASMA_CONDUCTOR, 252}},
            {
                    {RESSOURCE::ENERGY_CELL, 240},
                    {RESSOURCE::GRAPHENE, 384},
                    {RESSOURCE::SUPERFLUID_COOLANT, 560}
            },
            {
                    {RESSOURCE::CLAYTRONIC, 473},
                    {RESSOURCE::ENERGY_CELL, 946},
                    {RESSOURCE::HULL_PART, 1732},
            },
            225
    };

    const t_module_list MODULES{
            ARGON_L_STORAGE_CONTAINER,
            ARGON_L_STORAGE_SOLID,
            ARGON_L_STORAGE_LIQUID,
            ARGON_PIER_1,
            ARGON_BASIC_DOCK_3M_6S,
            ARGON_L_HABITAT,
            SOLAR_POWER_PLANT,
            GRAPHENE_PRODUCTION,
            REFINED_METHAL_PRODUCTION,
            HULL_PART_FACTORY,
            CLAYTRONICS_PRODUCTION,
            ANTIMATTER_CELL_PRODUCTION,
            MICROCHIP_PRODUCTION,
            QUANTUM_TUBE_PRODUCTION,
            SILICON_WAFER_PRODUCTION,
            SUPERFLUID_COOLANT_PRODUCTION,
    };

}


const std::map<RESSOURCE, Module> ressourcesMap = {
        {RESSOURCE::ENERGY_CELL,        MODULES::SOLAR_POWER_PLANT},
        {RESSOURCE::FOOD_RATIONS,       MODULES::FOOD_RATION_PRODUCTION},
        {RESSOURCE::GRAPHENE,           MODULES::GRAPHENE_PRODUCTION},
        {RESSOURCE::HULL_PART,          MODULES::HULL_PART_FACTORY},
        {RESSOURCE::MEAT,               MODULES::MEAT_PRODUCTION},
        {RESSOURCE::MEDICAL_SUPPLIES,   MODULES::ARGON_MEDICAL_SUPPLY_PRODUCTION},
        {RESSOURCE::REFINED_METAL,      MODULES::REFINED_METHAL_PRODUCTION},
        {RESSOURCE::SPICES,             MODULES::SPICE_PRODUCTION},
        {RESSOURCE::WATER,              MODULES::WATER_PRODUCTION},
        {RESSOURCE::WHEAT,              MODULES::WHEAT_PRODUCTION},
        {RESSOURCE::CLAYTRONIC,         MODULES::CLAYTRONICS_PRODUCTION},
        {RESSOURCE::ANTIMATTER_CELL,    MODULES::ANTIMATTER_CELL_PRODUCTION},
        {RESSOURCE::MICROCHIP,          MODULES::MICROCHIP_PRODUCTION},
        {RESSOURCE::QUANTUM_TUBE,       MODULES::QUANTUM_TUBE_PRODUCTION},
        {RESSOURCE::SILICON_WAFER,      MODULES::SILICON_WAFER_PRODUCTION},
        {RESSOURCE::SUPERFLUID_COOLANT, MODULES::SUPERFLUID_COOLANT_PRODUCTION},
};

const std::map<RESSOURCE, std::string> ressourcesNames = {
        {RESSOURCE::ENERGY_CELL,          "Energy Cell"},
        {RESSOURCE::ORE,                  "Ore"},
        {RESSOURCE::SILICON,              "Silicon"},
        {RESSOURCE::ICE,                  "Ice"},
        {RESSOURCE::WATER,                "Water"},
        {RESSOURCE::SPICES,               "Spices"},
        {RESSOURCE::WHEAT,                "Wheat"},
        {RESSOURCE::FOOD_RATIONS,         "Food Rations"},
        {RESSOURCE::MEDICAL_SUPPLIES,     "Medical Supplies"},
        {RESSOURCE::MEAT,                 "Meat"},
        {RESSOURCE::METHANE,              "Methane"},
        {RESSOURCE::HYDROGEN,             "Hydrogen"},
        {RESSOURCE::HELIUM,               "Helium"},
        {RESSOURCE::GRAPHENE,             "Graphene"},
        {RESSOURCE::REFINED_METAL,        "Refined Metal"},
        {RESSOURCE::HULL_PART,            "Hull Part"},
        {RESSOURCE::CLAYTRONIC,           "Claytronic"},
        {RESSOURCE::ANTIMATTER_CONVERTER, "Antimatter Converter"},
        {RESSOURCE::ANTIMATTER_CELL,      "Antimatter Cell"},
        {RESSOURCE::MICROCHIP,            "Microchip"},
        {RESSOURCE::QUANTUM_TUBE,         "Quantum Tube"},
        {RESSOURCE::SUPERFLUID_COOLANT,   "Superfluid Coolant"},
        {RESSOURCE::SILICON_WAFER,        "Silicon Wafer"},
};

std::string genModulePlan(const std::string &plan_name, const t_module_list &modules, StationSize station_size)
{
    std::stringstream              plan;
    int                            xpos_pierr = 2500 - 1000 * station_size.x_minus;
    int                            xpos_dock  = 2700 - 1000 * station_size.x_minus;
    std::stack<std::array<int, 3>> positions_dock{};
    std::stack<std::array<int, 3>> positions_pier{};

    // meters to kilometers
    station_size.x_plus *= 1000;
    station_size.x_minus *= 1000;
    station_size.y_plus *= 1000;
    station_size.y_minus *= 1000;
    station_size.z_plus *= 1000;
    station_size.z_minus *= 1000;

    for (int i = 200 - station_size.y_minus; i < station_size.y_plus; i += 2350) {
        for (int j = 200 - station_size.z_minus; j < station_size.z_plus; j += 2350) {
            positions_dock.emplace(std::array<int, 3>{xpos_dock, i, j});
            positions_pier.emplace(std::array<int, 3>{xpos_pierr, i, j});
        }
    }

    plan << R"(<?xml version="1.0" encoding="UTF-8"?>)" << std::endl;
    plan << R"(<plans>)" << std::endl;
    plan << R"(  <plan id="E678E734-BA20-4345-85BE-FCDD151DED9C_1619859416" name=")" << plan_name
         << R"(" description="">)" << std::endl;

    unsigned int    i = 1;
    for (auto const &module: modules) {
        std::array<int, 3> pos{0, 0, 0};
        if (module.type == MODULE_TYPE::PIER) {
            pos = positions_pier.top();
            positions_pier.pop();
        } else if (module.type == MODULE_TYPE::DOCK) {
            pos = positions_dock.top();
            positions_dock.pop();
        }

        plan << "    <entry " << "index=\"" << i << "\" macro=\"" << module.macro << "\">" << std::endl;
        plan << "      <offset>" << std::endl;
        plan << R"(        <position x=")" << pos[0] << R"(" y=")" << pos[1] << R"(" z=")" << pos[2] << R"("/>)"
             << std::endl;
//        plan << R"(        <position x="0" y="0" z="0"/>)" << std::endl;
        plan << "      </offset>" << std::endl;
        plan << "    </entry>" << std::endl;
    }

    plan << R"(  </plan>)" << std::endl;
    plan << R"(</plans>)" << std::endl;
    return plan.str();
}
