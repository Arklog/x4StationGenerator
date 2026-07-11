//
// Created by pierre on 7/11/26.
//

#ifndef X4STATIONGENERATOR__STORAGE_HPP
#define X4STATIONGENERATOR__STORAGE_HPP
#include "Module.hpp"
#include <rfl/Flatten.hpp>

namespace common::types::module {
    struct Storage {
        using storage_type = std::string;
        rfl::Flatten<Module> module;

        storage_type type;
        size_t       capacity;
    };
}

#endif //X4STATIONGENERATOR__STORAGE_HPP
