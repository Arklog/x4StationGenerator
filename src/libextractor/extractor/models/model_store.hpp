//
// Created by pierre on 7/6/26.
//

#ifndef X4STATIONGENERATOR__MODEL_STORE_HPP
#define X4STATIONGENERATOR__MODEL_STORE_HPP

#include <filesystem>

#include "Wares.hpp"
#include "Structure.hpp"
#include "T.hpp"
#include "Waregroups.hpp"

namespace extractor::models {
    struct ModelStore {
        using path = std::filesystem::path;

        ModelStore(LangFile &&translations, const path &path);

        models::Wares                  wares;
        models::Waregroups             waregroups;
        std::vector<models::Structure> production_modules;
        std::vector<models::Structure> habitats;
        std::vector<models::Structure> dock_and_pierr;
        std::vector<models::Structure> storage;
        LangFile                       translations;

    private:
        void _load_production_modules(const path &path);

        void _translate_t();
    };
}

#endif //X4STATIONGENERATOR__MODEL_STORE_HPP
