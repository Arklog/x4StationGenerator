//
// Created by pierre on 2/2/25.
//

#ifndef X4STATIONGENERATOR_STATIONBUILDER_HPP
#define X4STATIONGENERATOR_STATIONBUILDER_HPP

#include <functional>
#include <random>
#include <ctime>
#include "modules.hpp"

typedef std::function<t_module_list(const t_modules &)> t_module_generator;

/**
 * Generate a basic build order
 * @param modules
 * @return
 */
t_module_list basic_generator(const t_modules &modules);

/**
 * Generate a build order based on module priority
 * @param modules
 * @return
 */
t_module_list priority_generator(const t_modules &modules);

/**
 * Randomly generate a build order
 * @param modules
 * @return
 */
t_module_list rand_generator(const t_modules &modules);

class StationBuilder {
private:
    t_modules     _modules;
    t_module_list _ordered;
public:
    explicit StationBuilder(const t_modules &modules);

    void generateBuildOrder(t_module_generator generator = basic_generator);

    [[nodiscard]] const t_module_list &get() const;
};

#endif //X4STATIONGENERATOR_STATIONBUILDER_HPP
