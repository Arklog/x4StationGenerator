//
// Created by pierre on 2/1/25.
//

#include <cmath>
#include "ModuleGenerator.hpp"

ModuleGenerator::ModuleGenerator(t_modules modules): _base(modules) {

}

void ModuleGenerator::generate() {
    t_ressources ressources;

    while (_canComplete(ressources)) {
        t_modules tmp;

        for (auto &ressource: ressources) {
            auto &module = ressourcesMap.at(ressource.first);
            auto qty = -ressource.second;
            double nmodules = static_cast<double>(qty) / module.ressources_produced.at(ressource.first);

            tmp[module] = std::ceil(nmodules);
        }
        addMap(this->_to_complete, tmp);
        ressources.clear();
    }
}

bool ModuleGenerator::_canComplete(t_ressources &ressources) {
    auto consumption = _getConsumption();

    for (auto iter: consumption) {
        if (iter.second < 0 && ressourcesMap.find(iter.first) != ressourcesMap.end()) {
            addItem(ressources, iter);
        }
    }
    return !ressources.empty();
}

std::map<RESSOURCES, int> ModuleGenerator::_getConsumption()  {
    std::map<RESSOURCES, int> ressources = {};
    std::map<Module, int> modules = {};

    addMap(modules, this->_base);
    addMap(modules, this->_to_complete);
    for (auto &iter: modules) {
        auto tmp = iter.first.getTotal(iter.second, false);
        addMap(ressources, tmp);
    }

    return ressources;
}

t_modules ModuleGenerator::get() const {
    t_modules tmp{};

    addMap(tmp, this->_base);
    addMap(tmp, this->_to_complete);

    return tmp;
}
