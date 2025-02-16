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

    void _pushAndComplete(const Module &module);

    bool _isComplete(t_ressources &missing, const Module &module) const;

    /**
     * Generate the ressource list from all module in stack
     * @param module
     * @return
     */
    t_ressources _getRessourcesFromOrdered(const Module &module) const;

    size_t _getAvailableWorkforce() const;

public:
    StationBuilder() = default;

    explicit StationBuilder(const t_modules &modules, bool workforce = true);

    void setModules(t_modules modules);

    void setWorkforce(bool workforce);

    void generateBuildOrder();

    size_t getWorkforce() const;

    [[nodiscard]] const t_module_list &get() const;

    const t_modules &getModulesMap() const;

    t_ressources getRessources() const;
};

#endif //X4STATIONGENERATOR_STATIONBUILDER_HPP
