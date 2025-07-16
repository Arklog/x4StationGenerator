//
// Created by pierre on 7/17/25.
//

#include "Modules.hpp"

static std::vector<TmpModule> _g_modules;

void setModules(std::vector<TmpModule> modules) {
    _g_modules = modules;
}

const std::vector<TmpModule> &getModules() {
    return _g_modules;
}
