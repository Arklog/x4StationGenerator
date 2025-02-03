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

#include "utils.hpp"

enum class RESSOURCES {
    METHANE,
    ORE,
    ENERGY_CELL,
    GRAPHENE,
    REFINED_METAL,
    HULL_PART,
};

struct Module {
    const std::string               name;
    const std::string               macro;
    const std::map<RESSOURCES, int> ressources_produced;
    const std::map<RESSOURCES, int> ressources_produced_max;
    const std::map<RESSOURCES, int> ressources_consumed;
    const int                       workforce_max;
    const double                    workforce_efficiency;

    [[nodiscard]] std::map<RESSOURCES, int> getTotal(int nmodules = 1, bool workforce_is_max = false) const;
};

bool operator<(const Module& a, const Module& b);

typedef std::map<RESSOURCES, int> t_ressources;
typedef std::map<Module, int> t_modules;
typedef std::vector<Module> t_module_list;

namespace MODULES {
    const Module ARGON_L_STORAGE_CONTAINER = {
            "Argon L Storage Container",
            "storage_arg_l_container_01_macro",
            {},
            {},
            {},
            0,
            0
    };

    const Module ARGON_L_STORAGE_SOLID = {
            "Argon L Storage Solid",
            "storage_arg_l_solid_01_macro",
            {},
            {},
            {},
            0,
            0
    };

    const Module ARGON_L_STORAGE_LIQUID = {
            "Argon L Storage Liquid",
            "storage_arg_l_liquid_01_macro",
            {},
            {},
            {},
            0,
            0
    };

    const Module ARGON_L_HABITAT = {
            "Argon L Habitat",
            "hab_arg_l_01_macro",
            {},
            {},
            {},
            0,
            0
    };

    const Module SOLAR_POWER_PLANT = {
            "Solar Power Plant",
            "prod_gen_energycells_macro",
            {{RESSOURCES::ENERGY_CELL, 10500}},
            {{RESSOURCES::ENERGY_CELL, 15015}},
            {},
            90,
            1.43
    };

    const Module GRAPHENE_PRODUCTION = {
            "Graphene Production",
            "prod_gen_graphene_macro",
            {{RESSOURCES::GRAPHENE, 1440}},
            {{RESSOURCES::GRAPHENE, 2102}},
            {
                    {RESSOURCES::METHANE, 4800},
                    {RESSOURCES::ENERGY_CELL, 1200}
            },
            180,
            0
    };

    const Module REFINED_METHAL_PRODUCTION = {
            "Refined Metals Production",
            "prod_gen_refinedmetals_macro",
            {{RESSOURCES::REFINED_METAL, 2112}},
            {{RESSOURCES::REFINED_METAL, 3020}},
            {
                    {RESSOURCES::ORE, 5760},
                    {RESSOURCES::ENERGY_CELL, 2160}
            },
            225,
            0
    };

    const Module HULL_PART_FACTORY = {
            "Hull Part Factory",
            "prod_gen_hullparts_macro",
            {{RESSOURCES::HULL_PART, 1176}},
            {{RESSOURCES::HULL_PART, 1611}},
            {
                    {RESSOURCES::ENERGY_CELL, 320},
                    {RESSOURCES::GRAPHENE, 160},
                    {RESSOURCES::REFINED_METAL, 1120},
                },
            270,
            1.36
    };

    extern const t_module_list MODULES;
}

const std::map<RESSOURCES, Module> ressourcesMap = {
        {RESSOURCES::ENERGY_CELL, MODULES::SOLAR_POWER_PLANT},
        {RESSOURCES::GRAPHENE, MODULES::GRAPHENE_PRODUCTION},
        {RESSOURCES::REFINED_METAL, MODULES::REFINED_METHAL_PRODUCTION},
        {RESSOURCES::HULL_PART, MODULES::HULL_PART_FACTORY},
};

const std::map<RESSOURCES, std::string> ressourcesNames = {
        {RESSOURCES::ENERGY_CELL, "Energy Cell"},
        {RESSOURCES::ORE, "Ore"},
        {RESSOURCES::METHANE, "Methane"},
        {RESSOURCES::GRAPHENE, "Graphene"},
        {RESSOURCES::REFINED_METAL, "Refined Metal"},
        {RESSOURCES::HULL_PART, "Hull Part"},
};



std::string genModulePlan(const std::string &plan_name, const t_module_list& modules);

#endif //X4STATIONGENERATOR_MODULES_HPP
