//
// Created by pierre on 7/16/25.
//

#include "Wares.hpp"

static std::vector<Ware> _g_wares{};

void setWares(std::vector<Ware> wares) {
    _g_wares = wares;
}

const std::vector<Ware> &getWares() {
    return _g_wares;
}