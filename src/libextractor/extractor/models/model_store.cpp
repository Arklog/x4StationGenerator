//
// Created by pierre on 7/6/26.
//

#include "model_store.hpp"

#include <spdlog/spdlog.h>

extractor::ModelStore::ModelStore(const path &wares_path, const path &       waregroups_path, const path &modules_path,
                                  const path &modulegroups_path, const path &t_path) {
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

    wares::load_file(wares_path, wares);
    spdlog::info("Loaded {} wares", wares.ware.size());
    waregroups::load_file(waregroups_path, waregroups);
    spdlog::info("Loaded {} waregroups", waregroups.group.size());
    modules::load_file(modules_path, modules);
    spdlog::info("Loaded {} modules", modules.module.size());
    modulegroups::load_file(modulegroups_path, modulegroups);
    spdlog::info("Loaded {} modulegroups", modulegroups.group.size());
    t::load_file(t_path, t);
}
