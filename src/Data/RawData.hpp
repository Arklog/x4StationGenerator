//
// Created by pierre on 7/15/25.
//

#ifndef WARE_H
#define WARE_H
#include <Data/ID.h>
#include <optional>
#include <string>

#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"

// typedef std::string t_ware_id;
using t_ware_quantity = std::unordered_map<t_ware_id, unsigned int, std::hash<std::string> >;

struct Price {
    unsigned int min; // minimum price
    unsigned int avg; // average price
    unsigned int max; // maximum price
};

struct WareGroup {
    t_ware_group_id id;   // ware group id
    std::string     name; // ware group name
    unsigned int    tier; // ware group tier
};

struct WareAmount {
    t_ware_id id;     // ware id
    long int  amount; // amount of ware
};

struct ProductionEffect {
    std::string type;    // effect type (e.g., "workforce"...)
    double      product; // effect value
};

struct ModuleProduction {
    std::string                   name;    // production name
    t_production_method_id        method;  // production method id
    std::vector<WareAmount>       wares;   // wares produced/consumed
    std::vector<ProductionEffect> effects; // production effects
    unsigned int                  time;    // production time in seconds
    long int                      amount;  // production amount

    bool operator==(const t_production_method_id &production_method_id) const;

    bool operator==(const std::string &name) const;

    double getWorkforceFactor() const;
};

struct Ware {
    t_ware_id                     id;
    std::string                   name;
    std::string                   transport;
    Price                         price;
    WareGroup                     group;
    std::vector<ModuleProduction> production;

    unsigned int volume;

    bool operator==(const t_ware_id &ware_id) const;
};

struct Module {
    t_module_id                           id;
    std::string                           name;
    std::string                           macro;
    std::optional<std::string>            type;
    std::optional<t_production_method_id> production_method;

    Price                       price;
    std::vector<Ware>           production;
    ModuleProduction            build_cost;
    std::optional<unsigned int> workforce_capacity;
    std::optional<unsigned int> workforce_max;
    std::optional<t_race_id>    race;

    bool operator==(const t_module_id &module_id) const;

    const Ware &getWare() const;

    const ModuleProduction &getProduction() const;

    t_ware_quantity getBuildCost() const;
};

void from_json(const nlohmann::json &j, Price &price);

void from_json(const nlohmann::json &j, WareGroup &ware_group);

void from_json(const nlohmann::json &, ProductionEffect &production_effect);

void from_json(const nlohmann::json &j, WareAmount &ware_amount);

void from_json(const nlohmann::json &j, ModuleProduction &m);

void from_json(const nlohmann::json &j, Ware &ware);

void from_json(const nlohmann::json &j, Module &m);

#endif // WARE_H
