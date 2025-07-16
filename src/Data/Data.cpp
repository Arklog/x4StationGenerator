//
// Created by pierre on 7/15/25.
//

#include "Data.h"
#include "nlohmann/json.hpp"

#include <fstream>
#include <utility>

#include "spdlog/spdlog.h"

void from_json(const nlohmann::json &j, Price &price) {
    try {
        spdlog::info("parsing price from json");

        price.min = j["min"].get<unsigned int>();
        price.max = j["max"].get<unsigned int>();
        price.avg = j["avg"].get<unsigned int>();
    } catch (std::exception &e) {
        spdlog::error("Failed to parse price from json: {}", e.what());
        throw;
    }
}

void from_json(const nlohmann::json &j, WareAmount &w) {
    try {
        spdlog::info("parsing ware amount from json");

        w.id = j["ware"].get<std::string>();
        w.amount = j["amount"].get<unsigned int>();
    } catch (std::exception &e) {
        spdlog::error("Failed to parse ware amount from json: {}", e.what());
        throw;
    }
}

void from_json(const nlohmann::json &j, ModuleProduction &m) {
    try {
        spdlog::info("parsing module production from json");

        m.name = j["name"].get<std::string>();
        m.method = j["method"].get<std::string>();
        m.time = j["time"].get<unsigned int>();
        m.amount = j["amount"].get<unsigned int>();
        m.wares = j["wares"].get<std::vector<WareAmount> >();

    } catch (std::exception &e) {
        spdlog::error("Failed to parse module production from json: {}", e.what());
        throw;
    }
}

void from_json(const nlohmann::json &j, ModuleProduct &p) {
    try {
        spdlog::info("parsing module product from json");

        p.id = j["id"].get<std::string>();
        p.production = j["production"].get<std::vector<ModuleProduction> >();
    } catch (std::exception &e) {
        spdlog::error("Failed to parse module product from json: {}", e.what());
        throw;
    }
}

void from_json(const nlohmann::json &j, TmpModule &m) {
    try {
        spdlog::info("parsing module from json");

        m.id = j["id"].get<std::string>();
        m.name = j["name"].get<std::string>();
        m.price = j["price"].get<Price>();
        m.macro = j["macro"].get<std::string>();
        m.description = j["description"].get<std::string>();
        m.product = j.contains("product")
                        ? j.at("product").get<std::vector<ModuleProduct> >()
                        : std::optional<std::vector<ModuleProduct> >{};
        m.type = j.contains("type") ? j["type"].get<std::string>() : std::optional<std::string>{};
    } catch (std::exception &e) {
        spdlog::error("Failed to parse module from json: {}", e.what());
        throw;
    }
}
