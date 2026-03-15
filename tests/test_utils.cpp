//
// Created by pierre on 3/13/26.
//

#include "test_utils.hpp"
#include "Data/WareModuleAndWorkforce.hpp"

Loader LOADER = {};

void loadData ()
{
  spdlog::set_level (spdlog::level::debug);
  LOADER.load();
  buildDataFrom (LOADER._modules_json);
  buildDataFrom (LOADER._workforce);
}