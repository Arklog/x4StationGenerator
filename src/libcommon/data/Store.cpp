//
// Created by pierre on 3/31/26.
//

#include "Store.hpp"

namespace common::data {
    void Store::add(types::module::Dock &&m) {
        auto &                       last = docks.add(std::move(m));
        types::module::ModuleWrapper wrapper(last);
        add(std::move(wrapper));
    }

    void Store::add(types::module::Pierr &&m) {
        auto &                       last = piers.add(std::move(m));
        types::module::ModuleWrapper wrapper(last);
        add(std::move(wrapper));
    }

    void Store::add(types::module::Habitat &&m) {
        auto &                       last = habitats.add(std::move(m));
        types::module::ModuleWrapper wrapper(last);
        add(std::move(wrapper));
    }

    void Store::add(types::module::Storage &&m) {
        auto &last = storages.add(std::move(m));
        add(types::module::ModuleWrapper(last));
    }

    void Store::add(types::module::ProductionModule &&m) {
        auto &last = production.add(std::move(m));
        add(types::module::ModuleWrapper(last));
    }

    void Store::add(types::module::ModuleWrapper &&m) {
        modules.add(std::move(m));
    }

    void Store::add(types::Ware &&m) {
        wares.add(std::move(m));
    }

    void Store::add(types::Workforce &&m) {
        workforce.add(std::move(m));
    }
}
