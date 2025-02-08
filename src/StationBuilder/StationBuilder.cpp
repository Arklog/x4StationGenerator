//
// Created by pierre on 2/2/25.
//

#include "StationBuilder.hpp"

#include <utility>

StationBuilder::StationBuilder(const t_modules &modules, bool workforce) : _base_modules_map(modules),
                                                                           _end_modules_map{},
                                                                           _ordered{},
                                                                           _ressources_produced{},
                                                                           _workforce(workforce) {
    this->generateBuildOrder();
}

void StationBuilder::generateBuildOrder() {
    _ordered.clear();
    std::stack<Module> stack{};
    t_ressources       produced{};

    // Add dock and storage modules to the vector
    for (auto const &module: _base_modules_map) {
        if (module.first.type != MODULE_TYPE::DOCK && module.first.type != MODULE_TYPE::STORAGE) {
            continue;
        }
        int i = module.second;
        while (i--) {
            _ordered.push_back(module.first);
        }
//        _base_modules_map.erase(module.first);
    }

    // Generate build order for production modules
    for (auto const &module: _base_modules_map) {
        if (module.first.type != MODULE_TYPE::PRODUCTION)
            continue;
        int i = module.second;
        while (i--)
            this->_pushAndComplete(stack, produced, module.first);
    }

    // Add ordered production modules to the vector
    while (!stack.empty())
        _ordered.push_back(stack.top()), stack.pop();

    // Generate map
    this->_end_modules_map.clear();
    for (auto const &iter: _ordered) {
        this->_end_modules_map[iter] += 1;
    }

    this->_ressources_produced = produced;
}

const t_module_list &StationBuilder::get() const {
    return _ordered;
}

void StationBuilder::_pushAndComplete(std::stack<Module> &stack, t_ressources &produced, const Module &module) const {
    t_ressources missing{};
    stack.push(module);
    addRessources(produced, module.getTotal(1, this->_workforce));

    while (!_isComplete(produced, missing)) {

        for (auto const &iter: missing) {

            const auto &mod         = ressourcesMap.at(iter.first);
            auto       mod_produce  = mod.getTotal(1, this->_workforce).at(iter.first);
            int        missing_mods = std::ceil(std::abs(static_cast<double>(iter.second)) / mod_produce);

            while (missing_mods--) {
                stack.push(mod);
                addRessources(produced, mod.getTotal(1, this->_workforce));
            }
        }
    }
}

void StationBuilder::addRessources(t_ressources &ressources, const t_ressources &toAdd) {
    for (auto const &ressource: toAdd)
        ressources[ressource.first] += ressource.second;
}

bool StationBuilder::_isComplete(const t_ressources &produced, t_ressources &missing) const {
    missing.clear();
    for (auto const &ressource: produced) {

        if (ressource.second < 0 && ressourcesMap.find(ressource.first) != ressourcesMap.end())
            missing[ressource.first] = ressource.second;
    }
    return missing.empty();
}

const t_modules &StationBuilder::getModulesMap() const {
    return _end_modules_map;
}

const t_ressources &StationBuilder::getRessources() const {
    return _ressources_produced;
}

void StationBuilder::setModules(t_modules modules) {
    _base_modules_map = std::move(modules);
    this->generateBuildOrder();
}

void StationBuilder::setWorkforce(bool workforce) {
    _workforce = workforce;
    this->generateBuildOrder();
}
