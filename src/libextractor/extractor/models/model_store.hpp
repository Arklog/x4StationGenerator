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
#include "t.hpp"

namespace extractor {
    struct ModelStore {
        using path = std::filesystem::path;

        ModelStore(const path &wares_path, const path &       waregroups_path, const path &modules_path,
                   const path &modulegroups_path, const path &t_path);

        wares::waresType         wares;
        waregroups::groupsType   waregroups;
        modules::modulesType     modules;
        modulegroups::groupsType modulegroups;
        t::languageType          t;
    };
}

#endif //X4STATIONGENERATOR__MODEL_STORE_HPP
