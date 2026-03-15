#include "StationBuilder/Generator/ComplexGeneratorBase.hpp"

#include <catch2/catch_all.hpp>
#include "test_utils.hpp"
#include "Data/WareModuleAndWorkforce.hpp"

SCENARIO ("User want to generate a complex")
{
  GIVEN ("The datas are loaded")
  {
    loadData();
    Settings settings{
      .name = "",
      .workforce_enables = false,
      .workforce_module = getModuleIdFromName ("Argon L Habitat")
    };
    WareTargetContainer targetContainer{};

    THEN ("Nothing should be generated")
    {
      ComplexGeneratorBase generator{settings, targetContainer};

      REQUIRE (generator.build().empty());
    }

    WHEN("Targets are requested") {
      t_ware_id ware{"energycells"};
      targetContainer.setPrimaryTarget (ware);

      auto wareTarget = targetContainer.getTarget(ware);
      wareTarget->prodution = 1000;

      THEN("")
      {
	ComplexGeneratorBase generator{settings, targetContainer};
	auto complex = generator.build();
      }
    }
  }
}