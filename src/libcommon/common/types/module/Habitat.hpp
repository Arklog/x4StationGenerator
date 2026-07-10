//
// Created by pierre on 7/10/26.
//

#ifndef X4STATIONGENERATOR__HABITAT_HPP
#define X4STATIONGENERATOR__HABITAT_HPP
#include "Module.hpp"

namespace common::types::module {
    struct Habitat : Module {
        unsigned int capacity;
        std::string  race;
    };
}

#endif //X4STATIONGENERATOR__HABITAT_HPP
