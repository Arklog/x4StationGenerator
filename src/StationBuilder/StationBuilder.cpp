//
// Created by pierre on 2/2/25.
//

#include "StationBuilder.hpp"

#include <utility>

StationBuilder::StationBuilder(const BuildSettings &settings)
        : _base_modules_map{}, _end_modules_map{}, _ordered{}, _ressources_produced{}, _settings(settings)
{
}

void StationBuilder::generateBuildOrder()
{
    _ordered.clear();
    std::vector<Module> stack{};
    t_ressources        produced{};

    // Add dock and storage modules to the vector
    for (auto const &module: _base_modules_map) {
        if (!(module.first.type & MODULE_TYPE::DOCK_PIER_STORAGE)) {
            continue;
        }
        int i = module.second;
        while (i--) {
            _ordered.push_back(module.first);
        }
    }

    // Generate build order for production modules
    for (auto const &module: _base_modules_map) {
        if (module.first.type != MODULE_TYPE::PRODUCTION)
            continue;
        int i = module.second;
        while (i--)
            this->_pushAndComplete(module.first);
    }

    // Generate map
    this->_end_modules_map.clear();
    for (auto const &iter: _ordered) {
        this->_end_modules_map[iter] += 1;
    }

    this->_ressources_produced = produced;
}

const t_module_list &StationBuilder::get() const
{
    return _ordered;
}

void StationBuilder::_pushAndComplete(const Module &module)
{
    t_ressources missing{};

    while (!_isComplete(missing, module)) {

        for (auto const &iter: missing) {

            const auto &mod         = ressourcesMap.at(iter.first);
            auto       mod_produce  = mod.getTotal(1, _settings.getWorkforce()).at(iter.first);
            int        missing_mods = std::ceil(std::abs(static_cast<double>(iter.second)) / mod_produce);

            while (missing_mods--) {
                _ordered.push_back(mod);
            }
        }
    }
    _ordered.push_back(module);

    while (getWorkforce() > _getAvailableWorkforce()) {
        _ordered.push_back(MODULES::ARGON_L_HABITAT);
    }
}

bool StationBuilder::_isComplete(t_ressources &missing, const Module &module) const
{
    auto produced = _getRessourcesFromOrdered(module);
    missing.clear();
    for (auto const &ressource: produced) {

        if (ressource.second < 0 && ressourcesMap.find(ressource.first) != ressourcesMap.end())
            missing[ressource.first] = ressource.second;
    }
    return missing.empty();
}

const t_modules &StationBuilder::getModulesMap() const
{
    return _end_modules_map;
}

t_ressources StationBuilder::getRessources() const
{
    t_ressources ressources{};
    size_t       workforce = 0;

    for (auto const &iter: _ordered) {
        addMap(ressources, iter.getTotal(1, _settings.getWorkforce()));
        workforce += iter.workforce_max;
    }

    if (!_settings.getWorkforce())
        return ressources;

    workforce = std::ceil(static_cast<double>(workforce) / 50);
    for (auto const &iter: WORKFORCE_CONSUMPTION_PER_50) {
        std::pair<const RESSOURCE, int> tmp(iter);
        tmp.second *= -static_cast<int>(workforce);
        addItem(ressources, tmp);
    }
    return ressources;
}

void StationBuilder::setModules(t_modules modules)
{
    _base_modules_map = std::move(modules);
    this->generateBuildOrder();
}

t_ressources StationBuilder::_getRessourcesFromOrdered(const Module &module) const
{
    t_ressources ressources{};
    size_t       workforce = 0;

    addMap(ressources, module.getTotal(1, _settings.getWorkforce()));
    workforce = module.workforce_max;
    for (auto const &iter: _ordered) {
        addMap(ressources, iter.getTotal(1, _settings.getWorkforce()));
        workforce += iter.workforce_max;
    }

    if (!_settings.getWorkforce())
        return ressources;

    workforce = std::ceil(static_cast<double>(workforce) / 50);
    for (auto const &iter: WORKFORCE_CONSUMPTION_PER_50) {
        std::pair<const RESSOURCE, int> tmp(iter);
        tmp.second *= -static_cast<int>(workforce);
        addItem(ressources, tmp);
    }
    return ressources;
}

size_t StationBuilder::getWorkforce() const
{
    size_t workforce{};

    if (!_settings.getWorkforce())
        return 0;

    for (auto const &iter: _ordered)
        workforce += iter.workforce_max;
    return workforce;
}

size_t StationBuilder::_getAvailableWorkforce() const
{
    size_t habitats{};

    for (auto iter: _ordered) {
        if (iter.type == MODULE_TYPE::HABITAT)
            habitats += 1;
    }

    return habitats * 4000;
}
