//
// Created by pierre on 7/4/26.
//

#ifndef X4STATIONGENERATOR__MODULE_HPP
#define X4STATIONGENERATOR__MODULE_HPP
#include <string>

namespace common {
    struct Module
    {
	typedef enum
	{
	    NONE = 0,
	    CONNECTION,
	    PRODUCTION,
	    HABITAT,
	    DOCK,
	    PIERR
	} module_type;

	static module_type to_module_type (const std::string &type);

	typedef enum
	{
	} module_production;

	using module_id = std::string;

	std::string name;
	module_id id;
    };
} // namespace common

#endif // X4STATIONGENERATOR__MODULE_HPP
