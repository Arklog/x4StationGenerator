//
// Created by pierre on 2/2/25.
//

#include "StationBuilder.hpp"

#include <utility>

StationBuilder::StationBuilder(const t_modules &modules, bool workforce) : _base_modules_map(modules),
                                                                           _end_modules_map{},
                                                                           _ordered{},
                                                                           _ressources_produced{},
                                                                           _workforce(workforce)
{
    this->generateBuildOrder();
}

void StationBuilder::generateBuildOrder()
{
    _ordered.clear();
    std::vector<Module> stack{};
    t_ressources        produced{};

    // Add dock and storage modules to the vector
    for (auto const &module: _base_modules_map)
    {
        if (!(module.first.type & MODULE_TYPE::DOCK_PIER_STORAGE))
        {
            continue;
        }
        int i = module.second;
        while (i--)
        {
            _ordered.push_back(module.first);
        }
    }

    // Generate build order for production modules
    for (auto const &module: _base_modules_map)
    {
        if (module.first.type != MODULE_TYPE::PRODUCTION)
            continue;
        int i = module.second;
        while (i--)
            this->_pushAndComplete(module.first);
    }

    // Generate map
    this->_end_modules_map.clear();
    for (auto const &iter: _ordered)
    {
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

    while (!_isComplete(missing, module))
    {

        for (auto const &iter: missing)
        {

            const auto &mod         = ressourcesMap.at(iter.first);
            auto       mod_produce  = mod.getTotal(1, this->_workforce).at(iter.first);
            int        missing_mods = std::ceil(std::abs(static_cast<double>(iter.second)) / mod_produce);

            while (missing_mods--)
            {
                _ordered.push_back(mod);
            }
        }
    }
    _ordered.push_back(module);

    int missing_workforce = getWorkforce() - __getAvailableWorkforce();
    while (missing_workforce > 0)
    {
        _ordered.push_back(MODULES::ARGON_L_HABITAT);
        missing_workforce -= 4000;
    }
}

bool StationBuilder::_isComplete(t_ressources &missing, const Module &module) const
{
    auto produced = __getRessourcesFromOrdered(module);
    missing.clear();
    for (auto const &ressource: produced)
    {

        if (ressource.second < 0 && ressourcesMap.find(ressource.first) != ressourcesMap.end())
            missing[ressource.first] = ressource.second;
    }
    return missing.empty();
}

const t_modules &StationBuilder::getModulesMap() const
{
    return _end_modules_map;
}

const t_ressources &StationBuilder::getRessources() const
{
    return _ressources_produced;
}

void StationBuilder::setModules(t_modules modules)
{
    _base_modules_map = std::move(modules);
    this->generateBuildOrder();
}

void StationBuilder::setWorkforce(bool workforce)
{
    _workforce = workforce;
    this->generateBuildOrder();
}

t_ressources StationBuilder::__getRessourcesFromOrdered(const Module &module) const
{
    t_ressources ressources{};
    size_t       workforce = 0;

    addMap(ressources, module.getTotal(1, _workforce));
    workforce = module.workforce_max;
    for (auto const &iter: _ordered)
    {
        addMap(ressources, iter.getTotal(1, _workforce));
        workforce += iter.workforce_max;
    }

    if (!_workforce)
        return ressources;

    workforce = std::ceil(static_cast<double>(workforce) / 50);
    for (auto const &iter: WORKFORCE_CONSUMPTION_PER_50)
    {
        std::pair<const RESSOURCE, int> tmp(iter);
        tmp.second *= -static_cast<int>(workforce);
        addItem(ressources, tmp);
    }
    return ressources;
}

size_t StationBuilder::getWorkforce() const
{
    size_t workforce{};

    if (!_workforce)
        return 0;

    for (auto const &iter: _ordered)
        workforce += iter.workforce_max;
    return workforce;
}

size_t StationBuilder::__getAvailableWorkforce() const
{
    size_t habitats{};

    for (auto iter: _ordered)
    {
        if (iter.type == MODULE_TYPE::HABITAT)
            habitats += 1;
    }

    return habitats * 4000;
}
