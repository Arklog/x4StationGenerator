//
// Created by pierre on 7/26/25.
//
#include "defines.hpp"

namespace common::stationbuilder {
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

    bool Settings::operator==(const Settings &other) const {
        return this->name == other.name
               && this->sunlight == other.sunlight
               && this->workforce_enables == other.workforce_enables
               && this->workforce_module == other.workforce_module
               && this->docks == other.docks
               && this->storages == other.storages;
    }
}
