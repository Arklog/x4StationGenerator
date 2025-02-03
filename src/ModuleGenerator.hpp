//
// Created by pierre on 2/1/25.
//

#ifndef X4STATIONGENERATOR_MODULEGENERATOR_HPP
#define X4STATIONGENERATOR_MODULEGENERATOR_HPP

#include <vector>
#include <map>
#include "modules.hpp"

class ModuleGenerator {
private:
    t_modules _base;
    t_modules _to_complete;

    std::map<RESSOURCES, int> _getConsumption();
    bool _canComplete(t_ressources &ressources);
public:
    ModuleGenerator() = delete;
    ModuleGenerator(std::map<Module, int> modules);

    void generate();

    t_modules get() const;
};


#endif //X4STATIONGENERATOR_MODULEGENERATOR_HPP
