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


namespace MODULES {
    extern const Module ARGON_L_STORAGE_CONTAINER;
    extern const Module ARGON_L_STORAGE_SOLID;
    extern const Module ARGON_L_STORAGE_LIQUID;
    extern const Module ARGON_PIER_1;
    extern const Module ARGON_BASIC_DOCK_3M_6S;
    extern const Module ARGON_L_HABITAT;
    extern const Module SOLAR_POWER_PLANT;
    extern const Module WATER_PRODUCTION;
    extern const Module MEAT_PRODUCTION;
    extern const Module SPICE_PRODUCTION;
    extern const Module WHEAT_PRODUCTION;
    extern const Module FOOD_RATION_PRODUCTION;
    extern const Module ARGON_MEDICAL_SUPPLY_PRODUCTION;
    extern const Module GRAPHENE_PRODUCTION;
    extern const Module REFINED_METHAL_PRODUCTION;
    extern const Module HULL_PART_FACTORY;

    extern const t_module_list MODULES;
}

extern const std::map<RESSOURCE, Module> ressourcesMap;

extern const std::map<RESSOURCE, std::string> ressourcesNames;


std::string genModulePlan(const std::string &plan_name, const t_module_list &modules, StationSize size);

#endif //X4STATIONGENERATOR_MODULES_HPP
