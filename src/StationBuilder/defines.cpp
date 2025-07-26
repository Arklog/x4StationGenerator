//
// Created by pierre on 7/26/25.
//
#include "defines.hpp"

bool ModuleTarget::operator==(const ModuleTarget &other) const {
    return this->module_id == other.module_id;
}

bool ModuleTarget::operator==(const t_module_id &module_id) const {
    return this->module_id == module_id;
}
