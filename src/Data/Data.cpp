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
        price.min = j["min"].get<unsigned int>();
        price.max = j["max"].get<unsigned int>();
        price.avg = j["avg"].get<unsigned int>();
    } catch (std::exception &e) {
        spdlog::error("Failed to parse price from json: {}", e.what());
        throw std::runtime_error("Failed to parse price");
    }
}

void from_json(const nlohmann::json &j, WareAmount &w) {
    try {
        w.id = j["ware"].get<std::string>();
        w.amount = j["amount"].get<unsigned int>();
    } catch (std::exception &e) {
        spdlog::error("Failed to parse ware amount from json: {}", e.what());
        throw std::runtime_error("Failed to parse ware");
    }
}

void from_json(const nlohmann::json &j, ModuleProduction &m) {
    try {
        m.name = j["name"].get<std::string>();
        m.method = j["method"].get<std::string>();
        m.time = j["time"].get<unsigned int>();
        m.amount = j["amount"].get<unsigned int>();
        m.wares = j["ware"].get<std::vector<WareAmount> >();
    } catch (std::exception &e) {
        spdlog::error("Failed to parse module production from json: {}", e.what());
        throw std::runtime_error("Failed to parse module production");
    }
}

void from_json(const nlohmann::json &j, ModuleProduct &p) {
    try {
        p.id = j["id"].get<std::string>();
        p.production = j["production"].get<std::vector<ModuleProduction> >();
    } catch (std::exception &e) {
        spdlog::error("Failed to parse module product from json: {}", e.what());
        throw std::runtime_error("Failed to parse module product");
    }
}

void from_json(const nlohmann::json &j, TmpModule &m) {
    try {
        m.id = j["id"].get<std::string>();
        m.name = j["name"].get<std::string>();
        m.price = j["price"].get<Price>();
        m.macro = j["macro"].get<std::string>();
        m.description = j["description"].get<std::string>();
        m.product = j.contains("product") ? j.at("product").get<ModuleProduct>() : std::optional<ModuleProduct>{};
        m.type = j.contains("type") ? j["type"].get<std::string>() : std::optional<std::string>{};
    } catch (std::exception &e) {
        spdlog::error("Failed to parse module from json: {}", e.what());
        throw std::runtime_error("Failed to parse module from json");
    }
}
