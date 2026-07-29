//
// Created by pierre on 7/29/26.
//
#include "utils/WareTargetContainer.hpp"

#include <catch2/catch_all.hpp>

#include "test/data/Store.hpp"
#include "test/data/wares/Wares.hpp"

TEST_CASE("WareTargetContainer") {
    test::data::init();
    auto store = test::data::store;

    common::utils::WareTargetContainer wcontainer{store};

    REQUIRE(wcontainer.getPrimaryAndSecondaryTargets().empty());
    REQUIRE(wcontainer.getPrimaryTargets().empty());
    REQUIRE(wcontainer.getSecondaryTargets().empty());

    wcontainer.setPrimaryTarget(test::data::wares::ware_01.id);
    REQUIRE(wcontainer.getPrimaryTargets().size() == 1);
    REQUIRE(wcontainer.getSecondaryTargets().empty());
    REQUIRE(wcontainer.getPrimaryAndSecondaryTargets().size() == 1);

    wcontainer.unsetPrimaryTarget(test::data::wares::ware_01.id);
    REQUIRE(wcontainer.getPrimaryTargets().empty());
    REQUIRE(wcontainer.getPrimaryAndSecondaryTargets().empty());

    wcontainer.setSecondaryTarget(test::data::wares::ware_01.id);
    REQUIRE(wcontainer.getSecondaryTargets().size() == 1);
    REQUIRE(wcontainer.getPrimaryTargets().empty());
    REQUIRE(wcontainer.getPrimaryAndSecondaryTargets().size() == 1);
}
