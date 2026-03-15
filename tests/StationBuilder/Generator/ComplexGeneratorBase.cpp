#include "StationBuilder/Generator/ComplexGeneratorBase.hpp"

#include <catch2/catch_all.hpp>
#include "test_utils.hpp"
#include "Data/WareModuleAndWorkforce.hpp"
#include "ComplexInfoView.hpp"

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
      wareTarget->source_module = "module_gen_prod_energycells_01";

      THEN("")
      {
	ComplexGeneratorBase generator{settings, targetContainer};
	auto complex = generator.build();
	ComplexInfoView complexData{complex};

	const auto &summary = complexData.module_summary;

	REQUIRE(summary.size() == 1);
	REQUIRE(summary.at(t_module_id {"module_gen_prod_energycells_01"}) == 1);
      }
    }
  }
}