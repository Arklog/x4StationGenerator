//
// Created by pierre on 7/10/26.
//

#ifndef X4STATIONGENERATOR__PRICE_HPP
#define X4STATIONGENERATOR__PRICE_HPP
#include <cstddef>

namespace common::types {
    struct Price {
        size_t max;
        size_t min;
        size_t avg;
    };
}

#endif //X4STATIONGENERATOR__PRICE_HPP
