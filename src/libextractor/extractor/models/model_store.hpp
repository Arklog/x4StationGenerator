//
// Created by pierre on 7/6/26.
//

#ifndef X4STATIONGENERATOR__MODEL_STORE_HPP
#define X4STATIONGENERATOR__MODEL_STORE_HPP

#include <filesystem>

#include "Wares.hpp"
#include "Structure.hpp"

namespace extractor {
    struct ModelStore {
        using path = std::filesystem::path;

        ModelStore(const path &path);

        models::Wares                     wares;
        std::vector<structure::Structure> production_modules;
        std::vector<structure::Structure> habitats;
        std::vector<structure::Structure> dock_and_pierr;
        std::vector<structure::Structure> storage;

    private:
        void _load_production_modules(const path &path);
    };
}

#endif //X4STATIONGENERATOR__MODEL_STORE_HPP
