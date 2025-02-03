//
// Created by pierre on 2/2/25.
//

#ifndef X4STATIONGENERATOR_STATIONBUILDER_HPP
#define X4STATIONGENERATOR_STATIONBUILDER_HPP

#include <functional>
#include "modules.hpp"

typedef std::function<t_module_list (const t_modules &)> t_module_generator;

t_module_list basic_generator(const t_modules &modules);

class StationBuilder {
private:
    t_modules _modules;
    t_module_list _ordererd;
public:
    StationBuilder(const t_modules &modules);

    void generateBuildOrder(t_module_generator generator = basic_generator);

    const t_module_list &get() const;
};

#endif //X4STATIONGENERATOR_STATIONBUILDER_HPP
