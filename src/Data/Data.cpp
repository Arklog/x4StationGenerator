//
// Created by pierre on 7/15/25.
//

#include "Data.hpp"
#include "nlohmann/json.hpp"

#include <fstream>
#include <utility>

#include "spdlog/spdlog.h"

const Ware &TmpModule::getWare() const {
    return this->production[0];
}

const ModuleProduction &TmpModule::getProduction() const {
    const auto &production = this->build_cost.name;
    const auto &ware = this->production[0];

    for (const auto &ware_production: ware.production) {
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

        spdlog::info("parsed price from json");
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

        spdlog::info("parsed ware group from json");
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

        spdlog::info("parsed ware amount from json");
    } catch (std::exception &e) {
        spdlog::error("Failed to parse ware amount from json: {}", e.what());
        throw;
    }
}

void from_json(const nlohmann::json &j, ModuleProduction &m) {
    try {
        spdlog::info("parsing module production from json");

        m.name = j["name"].get<std::string>();
        spdlog::info("parsed module production name from json");
        // m.method = j["method"].get<std::string>();
        // spdlog::info("parsed module production method from json");
        m.time = j["time"].get<unsigned int>();
        spdlog::info("parsed module production time from json");
        m.amount = j["amount"].get<unsigned int>();
        spdlog::info("parsed module production amount from json");
        m.wares = j["wares"].get<std::vector<WareAmount> >();
        spdlog::info("parsed module production wares from json");

        spdlog::info("parsed module production from json");
    } catch (std::exception &e) {
        spdlog::error("Failed to parse module production from json: {}", e.what());
        throw;
    }
}

void from_json(const nlohmann::json &j, Ware &ware) {
    try {
        spdlog::info("parsing ware production from json");

        ware.id = j["id"].get<t_ware_id>();
        spdlog::info("parsed ware production id from json");
        ware.name = j["name"].get<std::string>();
        spdlog::info("parsed ware production name from json");
        // ware.description = j["description"].get<std::string>();
        // spdlog::info("parsed ware production description from json");
        ware.volume = j["volume"].get<unsigned int>();
        spdlog::info("parsed ware production volume from json");
        ware.transport = j["transport"].get<std::string>();
        spdlog::info("parsed ware production transport from json");
        ware.price = j["price"].get<Price>();
        spdlog::info("parsed ware production price from json");
        ware.group = j["group"].get<WareGroup>();
        spdlog::info("parsed ware production group from json");
        ware.production = j.contains("production")
                              ? j["production"].get<std::vector<ModuleProduction> >()
                              : std::vector<ModuleProduction>();
        spdlog::info("parsed ware production production from json");

        spdlog::info("parsed ware production from json");
    } catch (std::exception &e) {
        spdlog::error("Failed to parse ware production from json: {}", e.what());
        throw;
    }
}


void from_json(const nlohmann::json &j, TmpModule &m) {
    try {
        spdlog::info("parsing module from json");

        m.id = j["id"].get<t_module_id>();
        spdlog::info("parsed module id");
        m.name = j["name"].get<std::string>();
        spdlog::info("parsed module name");
        m.price = j["price"].get<Price>();
        spdlog::info("parsed module price");
        m.macro = j["macro"].get<std::string>();
        spdlog::info("parsed macro");
        m.production = j.contains("product")
                           ? j.at("product").get<std::vector<Ware> >()
                           : std::vector<Ware>{};
        spdlog::info("parsed module product");
        m.type = j.contains("type") ? j["type"].get<std::string>() : std::optional<std::string>{};
        spdlog::info("parsed module type");
        m.production_method = j.contains("production_method")
                                  ? j["production_method"].get<std::string>()
                                  : std::optional<std::string>{};
        m.build_cost = j["production"].get<std::vector<ModuleProduction> >()[0];
        spdlog::info("parsed module build_cost");

        if (m.production.size() > 1)
            throw std::runtime_error("No more than 1 ware produced");
    } catch (std::exception &e) {
        spdlog::error("Failed to parse module from json: {}", e.what());
        throw;
    }
}
