//
// Created by pierre on 7/6/26.
//

#ifndef X4STATIONGENERATOR__MODEL_STORE_HPP
#define X4STATIONGENERATOR__MODEL_STORE_HPP

#include <filesystem>

#include "wares.hpp"
#include "waregroups.hpp"
#include "modules.hpp"
#include "modulegroups.hpp"
#include "Structure.hpp"
#include "t.hpp"

namespace extractor {
    struct ModelStore {
        using path = std::filesystem::path;

        ModelStore(const path &path);

        wares::waresType                  wares;
        waregroups::groupsType            waregroups;
        modules::modulesType              modules;
        modulegroups::groupsType          modulegroups;
        t::languageType                   t;
        std::vector<structure::Structure> production_modules;
        std::vector<structure::Structure> habitats;
        std::vector<structure::Structure> dock_and_pierr;
        std::vector<structure::Structure> storage;

    private:
        void _load_production_modules(const path &path);
    };
}

#endif //X4STATIONGENERATOR__MODEL_STORE_HPP
