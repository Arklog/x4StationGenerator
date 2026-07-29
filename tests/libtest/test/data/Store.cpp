//
// Created by pierre on 7/29/26.
//

#include "Store.hpp"
#include "modules/Modules.hpp"
#include "wares/Wares.hpp"

static common::data::Store createStore() {
    common::data::Store store;

    auto module_01 = test::data::modules::prod_module_01;
    auto module_02 = test::data::modules::prod_module_02;
    store.add(std::move(module_01));
    store.add(std::move(module_02));

    auto ware_01 = test::data::wares::ware_01;
    auto ware_02 = test::data::wares::ware_02;
    store.add(std::move(ware_01));
    store.add(std::move(ware_02));

    return store;
}

namespace test::data {
    common::data::Store store = createStore();
}
