//
// Created by pierre on 7/15/25.
//

#ifndef LOADER_H
#define LOADER_H

#include "nlohmann/json.hpp"

class Loader {
private:
    std::vector<nlohmann::json> _wares_json;
    std::vector<nlohmann::json> _modules_json;

    void _load_wares();
    void _load_modules();

public:
    Loader() = default;

    void load();
};



#endif //LOADER_H
