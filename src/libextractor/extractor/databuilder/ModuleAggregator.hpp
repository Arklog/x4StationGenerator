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
#include "extractor/models/Waregroups.hpp"
#include "extractor/models/Wares.hpp"


namespace extractor::databuilder {
    struct ModuleBase {
        using macro_id   = std::string;
        using race_id    = std::string;
        using class_type = std::string;

        ModuleBase(models::Structure &&structure);

        macro_id               macro;
        std::string            name;
        class_type             module_class;
        std::optional<race_id> makerrace;
    };

    struct ProductionModule : ModuleBase {
        ProductionModule(models::Structure &&structure);

        struct ProducedWare {
            std::string ware;
            std::string method;
        };

        std::vector<ProducedWare> wares_produced;
        size_t                    workforce_max;;
    };

    struct Habitat : ModuleBase {
        Habitat(models::Structure &&structure);

        size_t  capacity;
        race_id workforce_race;
    };

    struct Dock : ModuleBase {
        Dock(models::Structure &&structure);
    };

    struct Storage : ModuleBase {
        Storage(models::Structure &&structure);

        size_t      storage_max;
        std::string storage_type;
    };

    struct Ware {
        using ware_id = std::string;

        struct Production {
            using method_id = std::string;

            Production(models::Wares::Ware::Production &&production);

            method_id                                method;
            double                                   time;
            size_t                                   amount;
            std::vector<std::pair<ware_id, size_t> > wares_required;
            std::unordered_map<std::string, double>  effects;
        };

        struct Price {
            Price(models::Wares::Ware::WarePrice &&price);

            size_t max;
            size_t min;
            size_t avg;
        };

        Ware(models::Wares::Ware &&ware);

        ware_id                                     id;
        std::string                                 group;
        size_t                                      tier;
        std::string                                 name;
        Price                                       price;
        std::unordered_map<std::string, Production> production;
        std::optional<std::string>                  component;
    };

    using Waregroup = models::Waregroups::Group;

    template<std::derived_from<ModuleBase> T>
    struct Aggregator {
        Aggregator(std::vector<models::Structure> &structures) {
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

        std::unordered_map<ProductionModule::macro_id, T> modules;
    };

    using ProductionModuleAggregator = Aggregator<ProductionModule>;
    using HabitatAggregator          = Aggregator<Habitat>;
    using DockAggregator             = Aggregator<Dock>;
    using StorageAggregator          = Aggregator<Storage>;

    struct WareAggregator {
        WareAggregator(models::Wares &wares);

        std::unordered_map<Ware::ware_id, Ware> by_id;
        std::unordered_map<Ware::ware_id, Ware> by_ref;
    };

    struct WaregroupsAggregator {
        WaregroupsAggregator(models::Waregroups &waregroups);

        std::unordered_map<std::string, Waregroup> waregroups;
    };
}

#endif //X4STATIONGENERATOR__MODULEBUILDER_HPP
