//
// Created by pierre on 2/2/25.
//

#include "StationBuilder.hpp"

StationBuilder::StationBuilder(const t_modules &modules): _modules(modules) {

}

void StationBuilder::generateBuildOrder(t_module_generator generator) {
    _ordererd = generator(_modules);
}

const t_module_list &StationBuilder::get() const {
    return _ordererd;
}

t_module_list basic_generator(const t_modules &modules) {
    t_module_list lst;

    for (auto const &module: modules) {
        int i = module.second;
        while (i--)
            lst.push_back(module.first);
    }

    return lst;
}
