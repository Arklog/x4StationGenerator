//
// Created by pierre on 7/10/26.
//

#ifndef X4STATIONGENERATOR__PIERR_HPP
#define X4STATIONGENERATOR__PIERR_HPP
#include "Module.hpp"
#include <rfl/Flatten.hpp>

namespace common::types::module {
    struct Pierr {
        rfl::Flatten<Module> module;
    };
}

#endif //X4STATIONGENERATOR__PIERR_HPP
