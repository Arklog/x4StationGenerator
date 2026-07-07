//
// Created by pierre on 7/6/26.
//

#include "model_store.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>
#include <spdlog/spdlog.h>
#include <rfl/xml.hpp>
#include <rfl/json.hpp>

static std::string read_file(const std::filesystem::path &path) {
    auto file = std::ifstream(path);
    auto data = file.rdbuf();
    auto ss   = std::stringstream();

    ss << data;
    auto str = ss.str();
    if (str.size() >= 3 && str.substr(0, 3) == "\xef\xbb\xbf")
        str.erase(0, 3);
    auto res = std::regex_replace(str, std::regex("\r"), "");
    return res;
}

extractor::ModelStore::ModelStore(const path &path) {
    auto wares_path        = path / "libraries/wares.xml";
    auto waregroups_path   = path / "libraries/waregroups.xml";
    auto modules_path      = path / "libraries/modules.xml";
    auto modulegroups_path = path / "libraries/modulegroups.xml";
    auto t_path            = path / "t/0001-l044.xml";

    if (!std::filesystem::exists(wares_path))
        throw std::runtime_error(fmt::format("Wares file {} does not exist", wares_path.string()));
    if (!std::filesystem::exists(waregroups_path))
        throw std::runtime_error(fmt::format("Waregroups file {} does not exist", waregroups_path.string()));
    if (!std::filesystem::exists(modules_path))
        throw std::runtime_error(fmt::format("Modules file {} does not exist", modules_path.string()));
    if (!std::filesystem::exists(modulegroups_path))
        throw std::runtime_error(fmt::format("Modulegroups file {} does not exist", modulegroups_path.string()));
    if (!std::filesystem::exists(t_path))
        throw std::runtime_error(fmt::format("Translation file {} does not exist", t_path.string()));

    // auto file = std::ifstream(wares_path);
    wares::load_data(read_file(wares_path), wares);
    spdlog::info("Loaded {} wares", wares.ware.size());
    waregroups::load_data(read_file(waregroups_path), waregroups);
    spdlog::info("Loaded {} waregroups", waregroups.group.size());
    modules::load_data(read_file(modules_path), modules);
    spdlog::info("Loaded {} modules", modules.module.size());
    modulegroups::load_data(read_file(modulegroups_path), modulegroups);
    spdlog::info("Loaded {} modulegroups", modulegroups.group.size());
    t::load_data(read_file(t_path), t);

    _load_production_modules(path);
}

void extractor::ModelStore::_load_production_modules(const path &path) {
    auto const struct_path = path / "assets/structures";
    struct structure_target {
        std::string                                    name;
        const std::filesystem::path                    dir;
        std::vector<structure::Structure> ModelStore::*vec;
    };
    static std::vector<structure_target> structures{
        {"production", struct_path / "production/macros", &ModelStore::production_modules},
        {"habitat", struct_path / "habitat/macros", &ModelStore::habitats},
        {"dock", struct_path / "dock/macros", &ModelStore::dock_and_pierr},
        {"storage", struct_path / "storage/macros", &ModelStore::storage},
    };

    for (auto const &struct_target: structures) {
        const auto &p = struct_target.dir;
        if (!std::filesystem::exists(p))
            throw std::runtime_error(fmt::format("Structure file {} does not exist", p.string()));

        auto it = std::filesystem::directory_iterator(p);
        for (auto const &item: it) {
            if (!item.is_regular_file() || item.path().extension() != ".xml")
                continue;

            auto prod_module = rfl::xml::load<structure::Structure>(item.path());
            if (!prod_module) {
                std::string fcontent = read_file(item.path());
                spdlog::error("Failed to load {}: {}\n{}", item.path().string(), prod_module.error().what(), fcontent);
                continue;
            }
            (this->*struct_target.vec).push_back(std::move(prod_module.value()));
        }

        spdlog::info("Loaded {} {} modules", (this->*struct_target.vec).size(), struct_target.name);
    }
}
