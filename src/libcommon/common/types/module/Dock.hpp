//
// Created by pierre on 7/10/26.
//

#ifndef X4STATIONGENERATOR__DOCK_HPP
#define X4STATIONGENERATOR__DOCK_HPP
#include "Module.hpp"
#include <rfl/Flatten.hpp>

namespace common::types::module {
    struct Dock {
        rfl::Flatten<Module> module;
    };
}

#endif //X4STATIONGENERATOR__DOCK_HPP
