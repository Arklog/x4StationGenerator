//
// Created by pierre on 7/15/25.
//

#include "RawData.hpp"
#include "nlohmann/json.hpp"

#include <fstream>
#include <utility>

#include "spdlog/spdlog.h"

bool ModuleProduction::operator==(const t_production_method_id &production_method_id) const {
    return this->method == production_method_id;
}

bool ModuleProduction::operator== (const std::string &name) const
{
  return this->method.raw () == name;
}

double ModuleProduction::getWorkforceFactor() const {
    for (auto i: effects) {
        if (i.type != "work")
            continue;
        return i.product + 1;
    }
    return 1;
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
        spdlog::error("[ModuleProduction]: {} no production method found", this->id);
        throw std::runtime_error("Production method not found");
    }

    const auto &production_method = this->production_method;
    const auto &ware = this->production[0].production;

    auto iter = std::find(ware.cbegin(), ware.cend(), production_method);
    if (iter != ware.cend()) {
        spdlog::error("[ModuleProduction]: {} no production method found, using default", this->id);
        return *iter;
    }

    iter = std::find(ware.cbegin(), ware.cend(), "default");
    if (iter == ware.cend()) {
        spdlog::error("[ModuleProduction]: {} no default production method found", this->id);
        throw std::out_of_range("No default production method found");
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
        ware_group.id = j["id"].get<t_ware_group_id>();
        ware_group.name = j["name"].get<std::string>();
        ware_group.tier = j.contains("tier") ? j["tier"].get<unsigned int>() : 0;
    } catch (std::exception &e) {
        spdlog::error("Failed to parse ware group id: {}", e.what());
        throw;
    }
}

void from_json(const nlohmann::json &j, ProductionEffect &production_effect) {
    try {
        production_effect.type = j["type"].get<std::string>();
        production_effect.product = j["product"].get<double>();
    } catch (std::exception &e) {
        spdlog::error("Failed to parse production effect type: {}", e.what());
        throw;
    }
}

void from_json(const nlohmann::json &j, WareAmount &ware_amount) {
    try {
        ware_amount.id = j["ware"].get<t_ware_id>();
        ware_amount.amount = j["amount"].get<unsigned int>();
    } catch (std::exception &e) {
        spdlog::error("Failed to parse ware amount from json: {}", e.what());
        throw;
    }
}

void from_json(const nlohmann::json &j, ModuleProduction &m) {
    try {
        m.name = j["name"].get<std::string>();
        m.method = j["method"].get<t_production_method_id>();
        m.time = j["time"].get<unsigned int>();
        m.amount = j["amount"].get<unsigned int>();
        m.wares = j["wares"].get<std::vector<WareAmount> >();
        m.effects = j["effects"];

        spdlog::debug("module production {} parsed", m.name);
    } catch (std::exception &e) {
        spdlog::error("Failed to parse module production from json: {}", e.what());
        throw;
    }
}

void from_json(const nlohmann::json &j, Ware &ware) {
    try {
        ware.id = j["id"].get<t_ware_id>();
        ware.name = j["name"].get<std::string>();
        // ware.description = j["description"].get<std::string>();
        // spdlog::debug("parsed ware production description from json");
        ware.volume = j["volume"].get<unsigned int>();
        ware.transport = j["transport"].get<std::string>();
        ware.price = j["price"].get<Price>();
        ware.group = j["group"].get<WareGroup>();
        ware.production = j.contains("production")
                              ? j["production"].get<std::vector<ModuleProduction> >()
                              : std::vector<ModuleProduction>();

        spdlog::debug("ware {} parsed", ware.id.raw());
    } catch (std::exception &e) {
        spdlog::error("Failed to parse ware production from json: {}", e.what());
        throw;
    }
}

void from_json(const nlohmann::json &j, Module &m) {
    try {
        m.id = j["id"].get<t_module_id>();
        m.name = j["name"].get<std::string>();
        m.price = j["price"].get<Price>();
        m.macro = j["macro"].get<std::string>();
        m.production = j.contains("product")
                           ? j.at("product").get<std::vector<Ware> >()
                           : std::vector<Ware>{};
        m.type = j.contains("type")
                     ? j["type"].get<std::string>()
                     : std::optional<std::string>{};
        m.production_method = j.contains("production_method")
                                  ? j["production_method"].get<t_production_method_id>()
                                  : std::optional<t_production_method_id>{};
        m.build_cost = j["production"].get<std::vector<ModuleProduction> >()[0];
        m.workforce_capacity = j.contains("workforce_capacity")
                                   ? j["workforce_capacity"].get<unsigned int>()
                                   : std::optional<unsigned int>{};
        m.workforce_max = j.contains("workforce_max")
                              ? j["workforce_max"].get<unsigned int>()
                              : std::optional<unsigned int>{};
        m.race = j.contains("race")
                     ? j["race"].get<std::string>()
                     : std::optional<std::string>{};

        if (m.production.size() > 1)
            throw std::runtime_error("No more than 1 ware produced");

        spdlog::debug("module {} parsed", m.id);
    } catch (std::exception &e) {
        spdlog::error("Failed to parse module from json: {}", e.what());
        throw;
    }
}
