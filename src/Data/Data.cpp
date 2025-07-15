//
// Created by pierre on 7/15/25.
//

#include "Data.h"
#include "nlohmann/json.hpp"

#include <fstream>
#include <utility>

Ware Ware::load(const std::string &filename) {
    std::fstream file(filename, std::fstream::in);

    auto data = nlohmann::json::parse(file);
    return Ware{
        .id = data.at("id"),
        .name = data.at("name"),
        .volume = data.at("volume"),
        .price = {
            .min = data.at("price").at("min"),
            .avg = data.at("price").at("avg"),
            .max = data.at("price").at("max"),
        },
        .description = data.at("description"),
        .transport = data.at("transport")
    };
}
