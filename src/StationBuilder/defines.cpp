//
// Created by pierre on 7/26/25.
//
#include "defines.hpp"

const std::string ModuleType::pier{"pier"};
const std::string ModuleType::dock{"dockarea"};
const std::string ModuleType::storage{"storage"};
const std::string ModuleType::habitat{"habitation"};

bool ModuleTarget::operator==(const ModuleTarget &other) const {
    return this->module_id == other.module_id;
}

bool ModuleTarget::operator==(const t_module_id &module_id) const {
    return this->module_id == module_id;
}

const Settings default_settings{
    .name = "<station_name>"
};