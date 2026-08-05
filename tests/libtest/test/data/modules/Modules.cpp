//
// Created by pierre on 7/29/26.
//

#include "Modules.hpp"
#include <rfl/json.hpp>

template<typename T, typename StrType>
static T load_T(StrType str) {
    std::string tmp(str);
    auto        v = rfl::json::read<T>(str);

    if (!v.has_value())
        throw std::runtime_error("Failed to load json object: " + v.error().what());
    return v.value();
}

namespace test::data::modules {
    using ProductionModule = common::types::module::ProductionModule;

    ProductionModule prod_module_01;
    ProductionModule prod_module_02;

    void init() {
        prod_module_01 = load_T<ProductionModule>(PROD_MODULE_01);
        prod_module_02 = load_T<ProductionModule>(PROD_MODULE_02);
    }
}
