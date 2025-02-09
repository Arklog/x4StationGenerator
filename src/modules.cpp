//
// Created by pierre on 2/1/25.
//
#include "modules.hpp"

std::map<RESSOURCES, int> Module::getTotal(int nmodules, bool workforce_is_max) const {
    std::map<RESSOURCES, int>       tmp;
    const std::map<RESSOURCES, int> &r = workforce_is_max ? this->ressources_produced_max : this->ressources_produced;

    for (auto i: r) {
        tmp[i.first] = i.second * nmodules;
    }
    for (auto i: ressources_consumed) {
        tmp[i.first] = -i.second * nmodules;
    }

    return tmp;
}

bool operator<(const Module &a, const Module &b) {
    const char *ca = a.name.c_str();
    const char *cb = b.name.c_str();

    return std::strcmp(ca, cb) < 0;
}

const t_module_list MODULES::MODULES{
        ARGON_L_STORAGE_CONTAINER,
        ARGON_L_STORAGE_SOLID,
        ARGON_L_STORAGE_LIQUID,
        ARGON_PIER_1,
        ARGON_BASIC_DOCK_3M_6S,
        SOLAR_POWER_PLANT,
        GRAPHENE_PRODUCTION,
        REFINED_METHAL_PRODUCTION,
        HULL_PART_FACTORY
};

std::string genModulePlan(const std::string &plan_name, const t_module_list &modules) {
    std::stringstream plan;

    plan << R"(<?xml version="1.0" encoding="UTF-8"?>)" << std::endl;
    plan << R"(<plans>)" << std::endl;
    plan << R"(  <plan id="E678E734-BA20-4345-85BE-FCDD151DED9C_1619859416" name=")" << plan_name
         << R"(" description="">)" << std::endl;

    unsigned int    i = 1;
    for (auto const &module: modules) {
        plan << "    <entry " << "index=\"" << i << "\" macro=\"" << module.macro << "\">" << std::endl;
        plan << "      <offset>" << std::endl;
        plan << R"(        <position x="0" y="0" z="0"/>)" << std::endl;
        plan << "      </offset>" << std::endl;
        plan << "    </entry>" << std::endl;
    }

    plan << R"(  </plan>)" << std::endl;
    plan << R"(</plans>)" << std::endl;
    return plan.str();
}
