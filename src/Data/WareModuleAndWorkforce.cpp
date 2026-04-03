//
// Created by pierre on 7/17/25.
//

#include "WareModuleAndWorkforce.hpp"
#include "spdlog/spdlog.h"

std::vector<WareAmount> getWorkforceUsage (std::string race,
					   unsigned int workforce_amount,
					   const Store &store)
{
    try
    {
	std::vector<WareAmount> ware_amounts;
	auto pair_data = store.workforce.at (race);

	for (const auto &item : pair_data)
	{
	    const auto &ware = item.first;
	    const auto &consumption = item.second;

	    long int final_consumption
		= std::ceil (consumption * workforce_amount);
	    ware_amounts.emplace_back (ware, final_consumption);
	}

	return ware_amounts;
    }
    catch (const std::out_of_range &e)
    {
	spdlog::error ("could not find production method {}", race);
	throw;
    }
}
