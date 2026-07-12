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
        load_datas_from(_store.workforce.workforces, "workforce");
        load_datas_from(_store.wares.datas, "wares");
        load_datas_from(_store.docks, "docks");
        load_datas_from(_store.piers, "piers");
        load_datas_from(_store.habitats, "habitats");
        load_datas_from(_store.storages, "storage");
        load_datas_from(_store.production.datas, "productions");
    }
}
