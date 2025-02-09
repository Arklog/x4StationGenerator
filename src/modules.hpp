//
// Created by pierre on 2/1/25.
//

#ifndef X4STATIONGENERATOR_MODULES_HPP
#define X4STATIONGENERATOR_MODULES_HPP

#include <map>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <stack>
#include <array>

#include "defines.hpp"
#include "utils.hpp"

bool operator<(const Module &a, const Module &b);

typedef std::map<RESSOURCE, int> t_ressources;
typedef std::map<Module, int>    t_modules;
typedef std::vector<Module>      t_module_list;

namespace MODULES {
    const Module ARGON_L_STORAGE_CONTAINER = {
            "Argon L Storage Container",
            "storage_arg_l_container_01_macro",
            MODULE_TYPE::STORAGE,
            {},
            {},
            {},
            0,
    };

    const Module ARGON_L_STORAGE_SOLID = {
            "Argon L Storage Solid",
            "storage_arg_l_solid_01_macro",
            MODULE_TYPE::STORAGE,
            {},
            {},
            {},
            0,
    };

    const Module ARGON_L_STORAGE_LIQUID = {
            "Argon L Storage Liquid",
            "storage_arg_l_liquid_01_macro",
            MODULE_TYPE::STORAGE,
            {},
            {},
            {},
            0,
    };

    const Module ARGON_PIER_1 = {
            "Argon Pier 1",
            "pier_arg_harbor_02_macro",
            MODULE_TYPE::PIER,
            {},
            {},
            {},
            0,
    };

    const Module ARGON_BASIC_DOCK_3M_6S = {
            "Argon Basic Dock 3M 6S",
            "dockarea_arg_m_station_02_lowtech_macro",
            MODULE_TYPE::DOCK,
            {},
            {},
            {},
            0,
    };

    const Module ARGON_L_HABITAT = {
            "Argon L Habitat",
            "hab_arg_l_01_macro",
            MODULE_TYPE::HABITAT,
            {},
            {},
            {},
            0,
    };

    const Module SOLAR_POWER_PLANT = {
            "Solar Power Plant",
            "prod_gen_energycells_macro",
            MODULE_TYPE::PRODUCTION,
            {{RESSOURCE::ENERGY_CELL, 10500}},
            {{RESSOURCE::ENERGY_CELL, 15015}},
            {},
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
            270,
    };

    extern const t_module_list MODULES;
}

const std::map<RESSOURCE, Module> ressourcesMap = {
        {RESSOURCE::ENERGY_CELL,   MODULES::SOLAR_POWER_PLANT},
        {RESSOURCE::GRAPHENE,      MODULES::GRAPHENE_PRODUCTION},
        {RESSOURCE::REFINED_METAL, MODULES::REFINED_METHAL_PRODUCTION},
        {RESSOURCE::HULL_PART,     MODULES::HULL_PART_FACTORY},
};

const std::map<RESSOURCE, std::string> ressourcesNames = {
        {RESSOURCE::ENERGY_CELL,   "Energy Cell"},
        {RESSOURCE::ORE,           "Ore"},
        {RESSOURCE::METHANE,       "Methane"},
        {RESSOURCE::GRAPHENE,      "Graphene"},
        {RESSOURCE::REFINED_METAL, "Refined Metal"},
        {RESSOURCE::HULL_PART,     "Hull Part"},
};


std::string genModulePlan(const std::string &plan_name, const t_module_list &modules, StationSize size);

#endif //X4STATIONGENERATOR_MODULES_HPP
