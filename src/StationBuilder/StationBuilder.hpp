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
#include "BuildSettings.hpp"

class StationBuilder {
private:
    t_modules           _base_modules_map;
    t_modules           _end_modules_map;
    t_module_list       _ordered;
    t_ressources        _ressources_produced;
    const BuildSettings &_settings;

    void _pushAndComplete(const Module &module);

    bool _isComplete(t_ressources &missing, const Module &module) const;

    /**
     * Generate the ressource list from all module in stack
     * @param module
     * @return
     */
    [[nodiscard]] t_ressources _getRessourcesFromOrdered(const Module &module) const;

    [[nodiscard]] size_t _getAvailableWorkforce() const;

public:
    explicit StationBuilder(const BuildSettings &settings);

    void setModules(t_modules modules);

    void generateBuildOrder();

    [[nodiscard]] size_t getWorkforce() const;

    [[nodiscard]] const t_module_list &get() const;

    [[nodiscard]] const t_modules &getModulesMap() const;

    [[nodiscard]] t_ressources getRessources() const;
};

#endif //X4STATIONGENERATOR_STATIONBUILDER_HPP
