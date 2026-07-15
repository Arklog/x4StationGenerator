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

        // std::smatch results;
        // std::regex_search(type, results, std::regex("([a-z]+)"));
        // for (auto &match: results) {
        // storage_type.emplace(match.str());
        // }
        storage_type = std::move(type);
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

        auto &effects_ = production.effects.value().effect;
        for (auto &[type, value]: effects_) {
            this->effects.emplace(std::move(type.value()), std::move(value.value()));
        }
    }

    Ware::Price::Price(models::Wares::Ware::WarePrice &&price) :
    min{price.min.value()},
    max{price.max.value()},
    avg{price.average.value()} {
    }

    Ware::Ware(models::Wares::Ware &&ware) :
    id{std::move(ware.id.value())},
    name{std::move(ware.name.value())},
    price(std::move(ware.price)),
    group(std::move(ware.group.value().value())),
    tier{0} {
        for (auto &prod: ware.production) {
            auto key  = prod.method.value();
            auto item = Production{std::move(prod)};
            production.emplace(std::move(key), std::move(item));
        }
        auto &component_ = ware.component;
        if (component.has_value())
            component = std::move(component_.value().ref.value());
    }

    WareAggregator::WareAggregator(models::Wares &wares) {
        for (auto &ware_: wares.ware) {
            Ware ware{std::move(ware_)};
            if (ware_.component.has_value())
                this->by_ref.emplace(ware_.component.value().ref.value(), std::move(ware));
            else
                this->by_id.emplace(ware.id, std::move(ware));
        }
    }

    WaregroupsAggregator::WaregroupsAggregator(models::Waregroups &waregroups) {
        for (auto &group: waregroups.group) {
            auto key = group.id.value();
            this->waregroups.emplace(key, std::move(group));
        }
    }

    ProductionModule::ProductionModule(models::Structure &&structure) :
    ModuleBase(std::move(structure)) {
        auto &queue = structure.macro.properties.production.value().queue[0];
        if (queue.item.has_value()) {
            auto &items = queue.item.value();

            for (auto &item: items) {
                this->wares_produced.emplace_back(std::move(item.ware.value()), std::move(item.method.value().value()));
            }
        } else {
            this->wares_produced.emplace_back(std::move(queue.ware.value().value()),
                                              std::move(queue.method.value().value()));
        }
        workforce_max = structure.macro.properties.workforce.value().max.value().value();
    }
}
