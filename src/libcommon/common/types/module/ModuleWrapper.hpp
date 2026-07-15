//
// Created by pierre on 7/15/26.
//

#ifndef X4STATIONGENERATOR__MODULEWRAPPER_HPP
#define X4STATIONGENERATOR__MODULEWRAPPER_HPP
#include <functional>
#include <type_traits>
#include "Module.hpp"

namespace common::types::module {
    template<typename T>
    concept have_module = requires(T t) { { t.module.value() } -> std::convertible_to<Module>; };

    struct ModuleWrapper {
        template<have_module T>
        ModuleWrapper(T &module) :
        module(module.module.value()) {
        }

        std::reference_wrapper<Module> module;
    };
}

#endif //X4STATIONGENERATOR__MODULEWRAPPER_HPP
