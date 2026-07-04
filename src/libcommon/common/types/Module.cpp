//
// Created by pierre on 7/4/26.
//
#include "common/types/Module.hpp"

#include "spdlog/spdlog.h"

#include <stdexcept>
#include <unordered_map>

common::Module::module_type
common::Module::to_module_type (const std::string &type)
{
    static const std::unordered_map<std::string, module_type> type_map
	= {{"connection", CONNECTION},
	   {"production", PRODUCTION},
	   {"dock", DOCK},
	   {"pierr", PIERR}};

    try
    {
	return type_map.at (type);
    }
    catch (std::out_of_range &e)
    {
	return NONE;
    }
    catch (std::exception &e)
    {
	spdlog::error ("unknown module type {}", type);
	return NONE;
    }
}