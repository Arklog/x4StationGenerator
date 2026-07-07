//
// Created by pierre on 7/7/26.
//

#include "ModuleAggregator.hpp"

#include <regex>

#include "extractor/models/Structure.hpp"

namespace extractor::databuilder {
    ModuleBase::ModuleBase(structure::Structure &&structure) :
    macro(std::move(structure.macro.name.value())),
    name(std::move(structure.macro.properties.identification.name.value())),
    module_class(std::move(structure.macro.class_.value().value())),
    makerrace(std::move(structure.macro.properties.identification.makerrace.value())) {
    }


    Habitat::Habitat(structure::Structure &&structure) :
    ModuleBase(std::move(structure)),
    capacity(std::move(structure.macro.properties.workforce.value().capacity.value().value())),
    workforce_race(std::move(structure.macro.properties.workforce.value().race.value().value())) {
    }

    Dock::Dock(structure::Structure &&structure) :
    ModuleBase(std::move(structure)) {
    }

    Storage::Storage(structure::Structure &&structure) :
    ModuleBase(std::move(structure)),
    storage_max{std::move(structure.macro.properties.cargo.value().max.value())} {
        const auto &type = structure.macro.properties.cargo.value().tags.value();

        std::smatch results;
        std::regex_search(type, results, std::regex("([a-z]+)"));
        for (auto &match: results) {
            storage_type.emplace(match.str());
        }
    }

    Module::Module(structure::Structure &&structure) :
    ModuleBase(std::move(structure)) {
    }
}
