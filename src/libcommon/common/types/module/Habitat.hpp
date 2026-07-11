//
// Created by pierre on 7/10/26.
//

#ifndef X4STATIONGENERATOR__HABITAT_HPP
#define X4STATIONGENERATOR__HABITAT_HPP
#include "Module.hpp"
#include <rfl/Flatten.hpp>

namespace common::types::module {
    struct Habitat {
        rfl::Flatten<Module> module;
        unsigned int         capacity;
        std::string          race;
    };
}

#endif //X4STATIONGENERATOR__HABITAT_HPP
