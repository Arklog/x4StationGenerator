//
// Created by pierre on 7/4/26.
//

#ifndef X4STATIONGENERATOR__STORE_HPP
#define X4STATIONGENERATOR__STORE_HPP
#include "common/types/Module.hpp"

#include <vector>

namespace extractor {
    struct Store
    {
	std::vector<common::Module> modules;
    };
} // namespace extractor

#endif // X4STATIONGENERATOR__STORE_HPP
