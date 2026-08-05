//
// Created by pierre on 7/26/25.
//
#include "defines.hpp"

#include "common/types/StationSaveFile.hpp"

namespace common::stationbuilder {
    const std::string ModuleType::pier{"pier"};
    const std::string ModuleType::dock{"dockarea"};
    const std::string ModuleType::storage{"storage"};
    const std::string ModuleType::habitat{"habitation"};

    bool ModuleTarget::operator==(const ModuleTarget &other) const {
        return this->module_id == other.module_id;
    }

    bool ModuleTarget::operator==(const t_module_id &module_id) const {
        return this->module_id == module_id;
    }

    Settings::Settings(const data::Store &store) :
    name{},
    sunlight{1.0f},
    workforce_module{},
    workforce_enables{},
    ware_targets{store},
    docks{},
    storages{} {
    }

    Settings::Settings(types::StationSaveFile &&save_file, const data::Store &store) :
    name(std::move(save_file.name)),
    sunlight(save_file.sun),
    workforce_module(std::move(save_file.habitat_id)),
    workforce_enables(save_file.workforce),
    ware_targets(store),
    docks{},
    storages{} {
        std::ranges::for_each(save_file.docks, [&](auto &n) {
            this->docks.emplace_back(ModuleTarget{std::move(n.first), n.second});
        });
        std::ranges::for_each(save_file.storages, [&](auto &n) {
            this->storages.emplace_back(ModuleTarget{std::move(n.first), n.second});
        });
        std::ranges::for_each(save_file.primary_targets, [&](auto &n) {
            auto target = this->ware_targets.setPrimaryTarget(n.ware_id);
            *target     = std::move(n);
        });
        std::ranges::for_each(save_file.secondary_targets, [&](auto &n) {
            auto target = this->ware_targets.setSecondaryTarget(n.ware_id);
            *target     = std::move(n);
        });
    }

    bool Settings::operator==(const Settings &other) const {
        return this->name == other.name
               && this->sunlight == other.sunlight
               && this->workforce_enables == other.workforce_enables
               && this->workforce_module == other.workforce_module
               && this->docks == other.docks
               && this->storages == other.storages
               && this->ware_targets == other.ware_targets;
    }
}
