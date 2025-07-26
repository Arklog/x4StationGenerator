//
// Created by pierre on 7/15/25.
//

#ifndef WARE_H
#define WARE_H
#include <optional>
#include <string>

#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"

typedef std::string t_ware_id;
typedef std::string t_ware_group_id;
typedef std::string t_module_id;
typedef std::string t_production_method_id;

struct Price {
    unsigned int min;
    unsigned int avg;
    unsigned int max;
};

struct WareGroup {
    t_ware_group_id id;
    std::string name;
    unsigned int tier;
};

struct WareAmount {
    t_ware_id id;
    long int amount;
};

struct ModuleProduction {
    std::string name;
    t_production_method_id method;
    std::vector<WareAmount> wares;
    unsigned int time;
    long int amount;

    bool operator==(const t_production_method_id& production_method_id) const;
};

struct Ware {
    t_ware_id id;
    std::string name;
    // std::string description;
    std::string transport;
    Price price;
    WareGroup group;
    std::vector<ModuleProduction> production;

    unsigned int volume;

    bool operator==(const t_ware_id &ware_id) const;
};

struct Module {
    t_module_id id;
    std::string name;
    std::string macro;
    std::optional<std::string> type;
    std::optional<std::string> production_method;

    Price price;
    std::vector<Ware> production;
    ModuleProduction build_cost;

    bool operator==(const t_module_id& module_id) const;

    const Ware& getWare() const;
    const ModuleProduction& getProduction() const;
};

void from_json(const nlohmann::json &j, Price &price);

void from_json(const nlohmann::json &j, WareGroup &ware_group);

void from_json(const nlohmann::json &j, WareAmount &ware_amount);

void from_json(const nlohmann::json &j, ModuleProduction &m);

void from_json(const nlohmann::json &j, Ware &ware);

void from_json(const nlohmann::json &j, Module &m);

#endif //WARE_H
