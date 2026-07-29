//
// Created by pierre on 7/29/26.
//
#include "Wares.hpp"
#include <rfl/json.hpp>

namespace test::data::wares {
    using Ware = common::types::Ware;

    Ware ware_01;
    Ware ware_02;

    void init() {
        ware_01 = rfl::json::read<Ware>(WARE_01).value();
        ware_02 = rfl::json::read<Ware>(WARE_02).value();
    }
}
