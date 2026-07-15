//
// Created by pierre on 7/15/25.
//

#include "Loader.hpp"

#include <fstream>

#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"

static const std::filesystem::path module_path("assets/modules");
static const std::filesystem::path workforce_path("assets/workforce.json");

namespace common::data {
    Loader::Loader(Store &store, std::filesystem::path path) :
    _store(store),
    _path(path) {
    }

    void Loader::load() {
        load_datas_from<types::Workforce>("workforce");
        load_datas_from<types::Ware>("wares");
        load_datas_from<types::module::Dock>("docks");
        load_datas_from<types::module::Pierr>("piers");
        load_datas_from<types::module::Habitat>("habitats");
        load_datas_from<types::module::Storage>("storage");
        load_datas_from<types::module::ProductionModule>("productions");
    }
}
