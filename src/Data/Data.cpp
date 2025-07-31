//
// Created by pierre on 7/15/25.
//

#include "Data.hpp"
#include "nlohmann/json.hpp"

#include <fstream>
#include <utility>

#include "spdlog/spdlog.h"

bool ModuleProduction::operator==(const t_production_method_id &production_method_id) const {
    return this->method == production_method_id;
}

bool Ware::operator==(const t_ware_id &ware_id) const {
    return this->id == ware_id;
}

const Ware &Module::getWare() const {
    return this->production[0];
}

bool Module::operator==(const t_module_id &module_id) const {
    return this->id == module_id;
}

const ModuleProduction &Module::getProduction() const {
    if (!this->production_method.has_value()) {
        spdlog::error("{} no production method found", this->id);
        throw std::runtime_error("Production method not found");
    }

    const auto &production_method = this->production_method;
    const auto &ware = this->production[0].production;

    const auto iter = std::find(ware.cbegin(), ware.cend(), production_method);
    if (iter == ware.cend()) {
        spdlog::error("{} no production method found", this->id);
        throw std::runtime_error("Production method not found");
    }

    return *iter;
}

t_ware_quantity Module::getBuildCost() const {
    t_ware_quantity w_quantity{};

    for (const auto &[ware_id, ware_amount]: this->build_cost.wares) {
        w_quantity[ware_id] += ware_amount;
    }

    return w_quantity;
}

void from_json(const nlohmann::json &j, Price &price) {
    try {
        spdlog::debug("parsing price from json");

        price.min = j["min"].get<unsigned int>();
        price.max = j["max"].get<unsigned int>();
        price.avg = j["avg"].get<unsigned int>();

        spdlog::debug("parsed price from json");
    } catch (std::exception &e) {
        spdlog::error("Failed to parse price from json: {}", e.what());
        throw;
    }
}

void from_json(const nlohmann::json &j, WareGroup &ware_group) {
    try {
        spdlog::debug("parsing ware group from json");

        ware_group.id = j["id"].get<t_ware_id>();
        ware_group.name = j["name"].get<std::string>();
        ware_group.tier = j.contains("tier") ? j["tier"].get<unsigned int>() : 0;

        spdlog::debug("parsed ware group from json");
    } catch (std::exception &e) {
        spdlog::error("Failed to parse ware group id: {}", e.what());
        throw;
    }
}

void from_json(const nlohmann::json &j, ProductionEffect &production_effect) {
    try {
        spdlog::debug("parsing production effect from json");

        production_effect.type = j["type"].get<std::string>();
        spdlog::debug("parsed production effect from json");
        production_effect.product = j["product"].get<double>();
        spdlog::debug("parsed production effect from json");
    } catch (std::exception &e) {
        spdlog::error("Failed to parse production effect type: {}", e.what());
        throw;
    }
}


void from_json(const nlohmann::json &j, WareAmount &ware_amount) {
    try {
        spdlog::debug("parsing ware amount from json");

        ware_amount.id = j["ware"].get<t_ware_id>();
        ware_amount.amount = j["amount"].get<unsigned int>();

        spdlog::debug("parsed ware amount from json");
    } catch (std::exception &e) {
        spdlog::error("Failed to parse ware amount from json: {}", e.what());
        throw;
    }
}

void from_json(const nlohmann::json &j, ModuleProduction &m) {
    try {
        spdlog::debug("parsing module production from json");

        m.name = j["name"].get<std::string>();
        spdlog::debug("parsed module production name from json");
        m.method = j["method"].get<std::string>();
        spdlog::debug("parsed module production method from json");
        m.time = j["time"].get<unsigned int>();
        spdlog::debug("parsed module production time from json");
        m.amount = j["amount"].get<unsigned int>();
        spdlog::debug("parsed module production amount from json");
        m.wares = j["wares"].get<std::vector<WareAmount> >();
        spdlog::debug("parsed module production wares from json");

        spdlog::debug("parsed module production from json");
    } catch (std::exception &e) {
        spdlog::error("Failed to parse module production from json: {}", e.what());
        throw;
    }
}

void from_json(const nlohmann::json &j, Ware &ware) {
    try {
        spdlog::debug("parsing ware production from json");

        ware.id = j["id"].get<t_ware_id>();
        spdlog::debug("parsed ware production id from json");
        ware.name = j["name"].get<std::string>();
        spdlog::debug("parsed ware production name from json");
        // ware.description = j["description"].get<std::string>();
        // spdlog::debug("parsed ware production description from json");
        ware.volume = j["volume"].get<unsigned int>();
        spdlog::debug("parsed ware production volume from json");
        ware.transport = j["transport"].get<std::string>();
        spdlog::debug("parsed ware production transport from json");
        ware.price = j["price"].get<Price>();
        spdlog::debug("parsed ware production price from json");
        ware.group = j["group"].get<WareGroup>();
        spdlog::debug("parsed ware production group from json");
        ware.production = j.contains("production")
                              ? j["production"].get<std::vector<ModuleProduction> >()
                              : std::vector<ModuleProduction>();
        spdlog::debug("parsed ware production production from json");

        spdlog::debug("parsed ware production from json");
    } catch (std::exception &e) {
        spdlog::error("Failed to parse ware production from json: {}", e.what());
        throw;
    }
}


void from_json(const nlohmann::json &j, Module &m) {
    try {
        spdlog::debug("parsing module from json");

        m.id = j["id"].get<t_module_id>();
        spdlog::debug("parsed module id");
        m.name = j["name"].get<std::string>();
        spdlog::debug("parsed module name");
        m.price = j["price"].get<Price>();
        spdlog::debug("parsed module price");
        m.macro = j["macro"].get<std::string>();
        spdlog::debug("parsed macro");
        m.production = j.contains("product")
                           ? j.at("product").get<std::vector<Ware> >()
                           : std::vector<Ware>{};
        spdlog::debug("parsed module product");
        m.type = j.contains("type") ? j["type"].get<std::string>() : std::optional<std::string>{};
        spdlog::debug("parsed module type");
        m.production_method = j.contains("production_method")
                                  ? j["production_method"].get<std::string>()
                                  : std::optional<std::string>{};
        m.build_cost = j["production"].get<std::vector<ModuleProduction> >()[0];
        spdlog::debug("parsed module build_cost");
        m.workforce = j.contains("workforce") ? j["workforce"].get<unsigned int>() : std::optional<unsigned int>{};
        spdlog::debug("parsed module workforce");
        m.race = j.contains("race") ? j["race"].get<std::string>() : std::optional<std::string>{};
        spdlog::debug("parsed module race");

        if (m.production.size() > 1)
            throw std::runtime_error("No more than 1 ware produced");
    } catch (std::exception &e) {
        spdlog::error("Failed to parse module from json: {}", e.what());
        throw;
    }
}
