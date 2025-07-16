//
// Created by pierre on 7/15/25.
//

#include "Data.h"
#include "nlohmann/json.hpp"

#include <fstream>
#include <utility>

static void from_json(const nlohmann::json& j, ModuleProduction& m) {
    m.name = j["name"].get<std::string>();
    m.method = j["method"].get<std::string>();
    m.time = j["time"].get<unsigned int>();
}

static void from_json(const nlohmann::json &j, Price& price) {
    price.min = j["min"].get<unsigned int>();
    price.max = j["max"].get<unsigned int>();
    price.avg = j["avg"].get<unsigned int>();
}

Ware Ware::load(const std::string &filename) {
    std::fstream file(filename, std::fstream::in);

    auto data = nlohmann::json::parse(file);
    return Ware{
        .id = data.at("id"),
        .name = data.at("name"),
        .volume = data.at("volume"),
        .price = data["price"].get<Price>(),
        .description = data.at("description"),
        .transport = data.at("transport")
    };
}

TmpModule TmpModule::load(const std::string &filename) {
    std::fstream file(filename, std::fstream::in);

    auto data = nlohmann::json::parse(file);
    return TmpModule{
        .id = data.at("id"),
        .name = data.at("name"),
        .macro = data.at("macro"),
        .description = data.at("description"),
        .type = data.find("type") != data.end()
                    ? std::optional<std::string>{data.at("type")}
                    : std::optional<std::string>{},

        .price = data["price"].get<Price>(),
        .production = data["production"].get<std::vector<ModuleProduction>>()
    };
}
