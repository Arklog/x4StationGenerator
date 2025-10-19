//
// Created by pierre on 7/15/25.
//

#ifndef LOADER_H
#define LOADER_H

#include "RawData.hpp"
#include "nlohmann/json.hpp"

class Loader {
private:
    /**
     * Load module files
     */
    void _loadModules();

    void _loadWorkforce();


    /**
     * Parse raw json ware
     */
    void _parse_wares();

public:
    Loader() = default;

    void load();


    std::vector<Ware> _wares_json;
    std::map<std::string, std::vector<std::pair<t_ware_id, double>>> _workforce;
    std::vector<Module> _modules_json;
};



#endif //LOADER_H
