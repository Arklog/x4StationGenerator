//
// Created by pierre on 2/2/25.
//

#ifndef X4STATIONGENERATOR_STATIONBUILDER_HPP
#define X4STATIONGENERATOR_STATIONBUILDER_HPP

#include <functional>
#include <random>
#include <stack>
#include <ctime>
#include <cmath>
#include "modules.hpp"
#include "utils.hpp"

class StationBuilder {
private:
    t_modules     _base_modules_map;
    t_modules     _end_modules_map;
    t_module_list _ordered;
    t_ressources  _ressources_produced;
    bool          _workforce;

    static void addRessources(t_ressources &ressources, const t_ressources &toAdd);

    void _pushAndComplete(std::vector<Module> &stack, t_ressources &produced, const Module &module) const;

    bool _isComplete(const t_ressources &produced, t_ressources &missing) const;

public:
    StationBuilder() = default;

    explicit StationBuilder(const t_modules &modules, bool workforce = false);

    void setModules(t_modules modules);

    void setWorkforce(bool workforce);

    void generateBuildOrder();

    [[nodiscard]] const t_module_list &get() const;

    const t_modules &getModulesMap() const;

    const t_ressources &getRessources() const;
};

#endif //X4STATIONGENERATOR_STATIONBUILDER_HPP
