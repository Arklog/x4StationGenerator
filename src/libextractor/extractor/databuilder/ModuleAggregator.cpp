//
// Created by pierre on 7/7/26.
//

#include "ModuleAggregator.hpp"

#include <regex>

#include "extractor/models/Structure.hpp"

namespace extractor::databuilder {
    ModuleBase::ModuleBase(models::Structure &&structure) :
    macro(std::move(structure.macro.name.value())),
    name(std::move(structure.macro.properties.identification.name.value())),
    module_class(std::move(structure.macro.class_.value().value())),
    makerrace(std::move(structure.macro.properties.identification.makerrace.value())) {
    }


    Habitat::Habitat(models::Structure &&structure) :
    ModuleBase(std::move(structure)),
    capacity(std::move(structure.macro.properties.workforce.value().capacity.value().value())),
    workforce_race(std::move(structure.macro.properties.workforce.value().race.value().value())) {
    }

    Dock::Dock(models::Structure &&structure) :
    ModuleBase(std::move(structure)) {
    }

    Storage::Storage(models::Structure &&structure) :
    ModuleBase(std::move(structure)),
    storage_max{structure.macro.properties.cargo.value().max.value()} {
        const auto &type = structure.macro.properties.cargo.value().tags.value();

        std::smatch results;
        std::regex_search(type, results, std::regex("([a-z]+)"));
        for (auto &match: results) {
            storage_type.emplace(match.str());
        }
    }

    Ware::Production::Production(models::Wares::Ware::Production &&production) :
    method(std::move(production.method.value())),
    time(production.time.value()),
    amount{production.amount.value()} {
        if (!production.primary.has_value())
            return;
        for (auto ware: production.primary.value().ware) {
            wares_required.emplace_back(std::move(ware.ware.value()), ware.amount.value());
        }
    }

    Ware::Ware(models::Wares::Ware &&ware) :
    id{std::move(ware.id.value())},
    name{std::move(ware.name.value())} {
        for (auto &prod: ware.production) {
            auto key  = prod.method.value();
            auto item = Production{std::move(prod)};
            production.emplace(std::move(key), std::move(prod));
        }
    }

    WareAggregator::WareAggregator(models::Wares &wares) {
        for (auto &ware_: wares.ware) {
            Ware ware{std::move(ware_)};
            this->wares.emplace(ware.id, std::move(ware));
        }
    }

    WaregroupsAggregator::WaregroupsAggregator(models::Waregroups &waregroups) {
        for (auto &group: waregroups.group) {
            auto key = group.id.value();
            this->waregroups.emplace(key, std::move(group));
        }
    }

    Module::Module(models::Structure &&structure) :
    ModuleBase(std::move(structure)) {
    }
}
