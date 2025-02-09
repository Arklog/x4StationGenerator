//
// Created by pierre on 2/1/25.
//
#include "modules.hpp"

std::map<RESSOURCE, int> Module::getTotal(int nmodules, bool workforce_is_max) const
{
    std::map<RESSOURCE, int>       tmp;
    const std::map<RESSOURCE, int> &r = workforce_is_max ? this->ressources_produced_max : this->ressources_produced;

    for (auto i: r)
    {
        tmp[i.first] = i.second * nmodules;
    }
    for (auto i: ressources_consumed)
    {
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

const t_module_list MODULES::MODULES{
        ARGON_L_STORAGE_CONTAINER,
        ARGON_L_STORAGE_SOLID,
        ARGON_L_STORAGE_LIQUID,
        ARGON_PIER_1,
        ARGON_BASIC_DOCK_3M_6S,
        ARGON_L_HABITAT,
        SOLAR_POWER_PLANT,
        GRAPHENE_PRODUCTION,
        REFINED_METHAL_PRODUCTION,
        HULL_PART_FACTORY
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

    for (int i = 200 - station_size.y_minus; i < station_size.y_plus; i += 2350)
    {
        for (int j = 200 - station_size.z_minus; j < station_size.z_plus; j += 2350)
        {
            positions_dock.emplace(std::array<int, 3>{xpos_dock, i, j});
            positions_pier.emplace(std::array<int, 3>{xpos_pierr, i, j});
        }
    }

    plan << R"(<?xml version="1.0" encoding="UTF-8"?>)" << std::endl;
    plan << R"(<plans>)" << std::endl;
    plan << R"(  <plan id="E678E734-BA20-4345-85BE-FCDD151DED9C_1619859416" name=")" << plan_name
         << R"(" description="">)" << std::endl;

    unsigned int    i = 1;
    for (auto const &module: modules)
    {
        std::array<int, 3> pos{0, 0, 0};
        if (module.type == MODULE_TYPE::PIER)
        {
            pos = positions_pier.top();
            positions_pier.pop();
        } else if (module.type == MODULE_TYPE::DOCK)
        {
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
