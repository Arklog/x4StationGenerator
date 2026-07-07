//
// Created by pierre on 7/7/26.
//

#ifndef X4STATIONGENERATOR__MODULEBUILDER_HPP
#define X4STATIONGENERATOR__MODULEBUILDER_HPP
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include <spdlog/spdlog.h>

#include "extractor/models/Structure.hpp"
#include "extractor/models/Wares.hpp"

namespace extractor::databuilder {
    struct ModuleBase {
        using macro_id   = std::string;
        using race_id    = std::string;
        using class_type = std::string;

        ModuleBase(structure::Structure &&structure);

        macro_id               macro;
        std::string            name;
        class_type             module_class;
        std::optional<race_id> makerrace;
    };

    struct Module : ModuleBase {
        Module(structure::Structure &&structure);
    };

    struct Habitat : ModuleBase {
        Habitat(structure::Structure &&structure);

        size_t  capacity;
        race_id workforce_race;
    };

    struct Dock : ModuleBase {
        Dock(structure::Structure &&structure);
    };

    struct Storage : ModuleBase {
        Storage(structure::Structure &&structure);

        size_t                storage_max;
        std::set<std::string> storage_type;
    };

    struct Ware {
        using ware_id = std::string;

        Ware(models::Wares &&wares);

        ware_id     id;
        std::string name;
    };

    template<std::derived_from<ModuleBase> T>
    struct Aggregator {
        Aggregator(std::vector<structure::Structure> &structures) {
            modules.reserve(structures.size());

            for (auto &item: structures) {
                try {
                    T tmp{std::move(item)};
                    modules.emplace(tmp.macro, std::move(tmp));
                } catch (const std::exception &e) {
                    spdlog::error("Failed to load {}: {}", item.macro.name.value(), e.what());
                }
            }
        }

        std::unordered_map<Module::macro_id, T> modules;
    };

    using ModuleAggregator  = Aggregator<Module>;
    using HabitatAggregator = Aggregator<Habitat>;
    using DockAggregator    = Aggregator<Dock>;
    using StorageAggregator = Aggregator<Storage>;
}

#endif //X4STATIONGENERATOR__MODULEBUILDER_HPP
