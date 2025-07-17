//
// Created by pierre on 7/15/25.
//

#ifndef WARE_H
#define WARE_H
#include <optional>
#include <string>

#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"

struct Price {
    unsigned int min;
    unsigned int avg;
    unsigned int max;
};

struct WareGroup {
    std::string id;
    std::string name;
    unsigned int tier;
};

struct WareAmount {
    std::string id;
    unsigned int amount;
};

struct ModuleProduction {
    std::string name;
    std::string method;
    std::vector<WareAmount> wares;
    unsigned int time;
    unsigned int amount;
};

struct Ware {
    std::string id;
    std::string name;
    std::string description;
    std::string transport;
    Price price;
    WareGroup group;
    std::vector<ModuleProduction> production;

    unsigned int volume;
};

struct TmpModule {
    std::string id;
    std::string name;
    std::string macro;
    std::string description;
    std::optional<std::string> type;

    Price price;
    std::optional<std::vector<Ware> > product;
};

void from_json(const nlohmann::json &j, Price &price);

void from_json(const nlohmann::json &j, WareGroup &ware_group);

void from_json(const nlohmann::json &j, WareAmount &w);

void from_json(const nlohmann::json &j, ModuleProduction &m);

void from_json(const nlohmann::json &j, Ware &ware);

void from_json(const nlohmann::json &j, TmpModule &m);

#endif //WARE_H
