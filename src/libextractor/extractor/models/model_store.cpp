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

#include "LangFile.hpp"

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

extractor::models::ModelStore::ModelStore(const path &path) {
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

    auto wares_ = rfl::xml::load<models::Wares>(wares_path);
    if (!wares_.has_value()) {
        auto &err = wares_.error();
        spdlog::error("Failed to load wares: {}", wares_.error().what());
        throw std::runtime_error("Failed to load wares");
    }
    this->wares = std::move(wares_.value());

    auto t_ = rfl::xml::load<models::T>(t_path);
    if (!t_.has_value()) {
        spdlog::error("Failed to load translation: {}", t_.error().what());
        throw std::runtime_error("Failed to load translation");
    }
    this->t = std::move(t_.value());

    auto waregroups_ = rfl::xml::load<models::Waregroups>(waregroups_path);
    if (!waregroups_.has_value()) {
        spdlog::error("Failed to load waregroups: {}", waregroups_.error().what());
        throw std::runtime_error("Failed to load waregroups");
    }
    this->waregroups = std::move(waregroups_.value());

    _load_production_modules(path);
    _translate_t();
}

void extractor::models::ModelStore::_load_production_modules(const path &path) {
    auto const struct_path = path / "assets/structures";
    struct structure_target {
        std::string                                 name;
        const std::filesystem::path                 dir;
        std::vector<models::Structure> ModelStore::*vec;
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

            auto prod_module = rfl::xml::load<models::Structure>(item.path());
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

void extractor::models::ModelStore::_translate_t() {
    LangFile lang(std::move(this->t));
    auto     translate_structures = [&lang](std::vector<models::Structure> &structures) {
        for (auto &structure: structures) {
            models::translate(lang, structure);
        }
    };

    models::translate(lang, this->wares);
    models::translate(lang, this->waregroups);
    translate_structures(this->production_modules);
    translate_structures(this->habitats);
    translate_structures(this->dock_and_pierr);
    translate_structures(this->storage);
}
