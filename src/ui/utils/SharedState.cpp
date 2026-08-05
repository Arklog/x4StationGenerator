//
// Created by pierre on 7/27/26.
//

#include "SharedState.hpp"

namespace ui::utils {
    SharedState::SharedState(const common::data::Store &store) :
    settings_(store),
    complex_() {
    }
}
