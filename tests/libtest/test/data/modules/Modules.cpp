//
// Created by pierre on 7/29/26.
//

#include "Modules.hpp"
#include <rfl/json.hpp>

namespace test::data::modules {
    const common::types::module::ProductionModule prod_module_01 = rfl::json::read<
        common::types::module::ProductionModule>(PROD_MODULE_01).value();
}
