//
// Created by pierre on 7/15/25.
//

#include "Loader.hpp"

#include <fstream>

#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"

static const std::filesystem::path module_path("assets/modules");
static const std::filesystem::path workforce_path("assets/workforce.json");
// static const auto module_path =
// QString("tools/regenerate_ware_and_modules/output/modules");

void Loader::_loadModules() {
    spdlog::info("Loading modules");

    auto it = std::filesystem::directory_iterator(module_path);
    for (const auto &file_iterator: it) {
        try {
            if (!file_iterator.is_regular_file() && file_iterator.path().extension() != ".json")
                continue;

            const auto &path = file_iterator.path();

            spdlog::info("loading: {}", path.filename().string());
            std::fstream file(absolute(path), std::fstream::in);

            if (!file.is_open())
                throw std::runtime_error("Failed to open file " + path.filename().string());
            if (file.bad()) {
                throw std::runtime_error("Failed to read file " + path.filename().string());
            }

            auto j      = nlohmann::json::parse(file);
            auto module = j.get<Module>();

            _store.registerModule(std::move(module));
            file.close();
        } catch (const std::exception &e) {
            spdlog::error("Could not parse {} : {}", file_iterator.path().string(), e.what());
        }
    }
}

void Loader::_loadWorkforce() {
    spdlog::info("Loading workforce");

    try {
        std::ifstream file(workforce_path, std::ifstream::in);
        if (!file.is_open())
            throw std::runtime_error("Failed to open file "
                                     + workforce_path.string());
        if (file.bad())
            throw std::runtime_error("Failed to read file "
                                     + workforce_path.string());

        auto j = nlohmann::json::parse(file);
        this->_store.workforce
                = j.get<std::map<std::string,
                    std::vector<std::pair<t_ware_id, double> > > >();
        file.close();

        spdlog::debug("found {} workforce", this->_store.workforce.size());
    } catch (const std::exception &e) {
        spdlog::error("could not parse workforce: {}", e.what());
        throw;
    }
}

void Loader::_parse_wares() {
}

Loader::Loader(Store &store) : _store(store) {
}

void Loader::load() {
    this->_loadModules();
    this->_loadWorkforce();
}
