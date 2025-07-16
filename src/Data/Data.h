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

struct Ware {
    std::string id;
    std::string name;
    std::string description;
    std::string transport;
    Price price;
    unsigned int volume;
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

struct ModuleProduct {
    std::string id;
    std::vector<ModuleProduction> production;
};

struct TmpModule {
    std::string id;
    std::string name;
    std::string macro;
    std::string description;
    std::optional<std::string> type;

    Price price;
    std::optional<ModuleProduct> product;
};

void from_json(const nlohmann::json &j, Price &price);

void from_json(const nlohmann::json &j, Ware &w);

void from_json(const nlohmann::json &j, WareAmount &w);

void from_json(const nlohmann::json &j, ModuleProduction &m);

void from_json(const nlohmann::json &j, TmpModule &m);

void from_json(const nlohmann::json &j, ModuleProduct &p);
#endif //WARE_H
