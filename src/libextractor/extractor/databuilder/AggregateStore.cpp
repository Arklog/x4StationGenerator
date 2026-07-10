//
// Created by pierre on 7/7/26.
//

#include "AggregateStore.hpp"

namespace extractor::databuilder {
    AggregateStore::AggregateStore(models::ModelStore &&model_store) :
    modules(model_store.production_modules),
    habitats(model_store.habitats),
    storages(model_store.storage),
    docks(model_store.dock_and_pierr),
    wares(model_store.wares),
    waregroups(model_store.waregroups) {
    }
} // extractor
