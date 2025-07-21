//
// Created by pierre on 7/15/25.
//

#include "Data.hpp"
#include "nlohmann/json.hpp"

#include <fstream>
#include <utility>

#include "spdlog/spdlog.h"

const Ware & TmpModule::getWare() const {
    return this->production[0];
}

const ModuleProduction & TmpModule::getProduction() const {
    const auto& production = this->build_cost.name;
    const auto& ware = this->production[0];

    for (const auto& ware_production: ware.production) {
        if (ware_production.name == production)
            return ware_production;
    }

    spdlog::error("{} no matching prodution method found for {}", this->id, production);
    throw std::runtime_error("Production not found");
}

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

void from_json(const nlohmann::json &j, WareGroup &ware_group) {
    try {
        spdlog::info("parsing ware group from json");

        ware_group.id = j["id"].get<t_ware_id>();
        ware_group.name = j["name"].get<std::string>();
        ware_group.tier = j.contains("tier") ? j["tier"].get<unsigned int>() : 0;
    } catch (std::exception &e) {
        spdlog::error("Failed to parse ware group id: {}", e.what());
        throw;
    }
}


void from_json(const nlohmann::json &j, WareAmount &ware_amount) {
    try {
        spdlog::info("parsing ware amount from json");

        ware_amount.id = j["ware"].get<t_ware_id>();
        ware_amount.amount = j["amount"].get<unsigned int>();
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

void from_json(const nlohmann::json &j, Ware &ware) {
    try {
        spdlog::info("parsing ware production from json");

        ware.id = j["id"].get<t_ware_id>();
        ware.name = j["name"].get<std::string>();
        ware.description = j["description"].get<std::string>();
        ware.volume = j["volume"].get<unsigned int>();
        ware.transport = j["transport"].get<std::string>();

        ware.price = j["price"].get<Price>();
        ware.group = j["group"].get<WareGroup>();
        ware.production = j.contains("production")
                              ? j["production"].get<std::vector<ModuleProduction> >()
                              : std::vector<ModuleProduction>();
    } catch (std::exception &e) {
        spdlog::error("Failed to parse ware production from json: {}", e.what());
        throw;
    }
}


void from_json(const nlohmann::json &j, TmpModule &m) {
    try {
        spdlog::info("parsing module from json");

        m.id = j["id"].get<t_module_id>();
        m.name = j["name"].get<std::string>();
        m.price = j["price"].get<Price>();
        m.macro = j["macro"].get<std::string>();
        m.description = j["description"].get<std::string>();
        m.production = j.contains("product")
                        ? j.at("product").get<std::vector<Ware> >()
                        : std::vector<Ware>{};
        m.type = j.contains("type") ? j["type"].get<std::string>() : std::optional<std::string>{};
        m.build_cost = j["production"].get<std::vector<ModuleProduction>>()[0];

        if (m.production.size() > 1)
            throw std::runtime_error("No more than 1 ware produced");
    } catch (std::exception &e) {
        spdlog::error("Failed to parse module from json: {}", e.what());
        throw;
    }
}
