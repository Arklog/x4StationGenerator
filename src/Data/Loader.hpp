//
// Created by pierre on 7/15/25.
//

#ifndef LOADER_H
#define LOADER_H

#include "Data.hpp"
#include "nlohmann/json.hpp"

class Loader {
private:
    /**
     * Load module files
     */
    void _load_modules();


    /**
     * Parse raw json ware
     */
    void _parse_wares();

public:
    Loader() = default;

    void load();


    std::vector<Ware> _wares_json;
    std::vector<TmpModule> _modules_json;
};



#endif //LOADER_H
