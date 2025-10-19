//
// Created by pierre on 10/19/25.
//

#include "Data.hpp"

std::shared_ptr<ModuleData>       Data::modules       = std::make_shared<ModuleData>();
std::shared_ptr<WareData>         Data::wares         = std::make_shared<WareData>();
std::shared_ptr<WareGroupData>    Data::ware_groups   = std::make_shared<WareGroupData>();
std::shared_ptr<RelationshipData> Data::relationships = std::make_shared<RelationshipData>();
std::shared_ptr<WorkforceData>    Data::workforce     = std::make_shared<WorkforceData>();

void Data::registerWare(const Ware &ware) {
    auto &vector   = wares->wares;
    auto &map      = wares->ware_map;
    auto &name_map = wares->ware_name_map;

    // A ware should not be registered twice
    if (map.contains(ware.id)) {
        spdlog::info("ware {} already registered", ware.id);
        return;
    }

    auto &item      = vector.emplace_back(ware);
    map[item.id]    = &item;
    name_map[&item] = item.name;

    Data::registerWareGroup(item.group);
}

void Data::registerWareGroup(const WareGroup &ware_group) {
    auto &vector = ware_groups->ware_groups;
    auto &map    = ware_groups->ware_group_map;

    // A ware group should not be registered twice
    if (map.contains(ware_group.id)) {
        spdlog::info("ware group {} already registered", ware_group.id);
        return;
    }

    auto &item   = vector.emplace_back(ware_group);
    map[item.id] = &item;
}

void Data::registerRelationship(const Module &module, const Ware &ware) {
    auto &ware_id   = ware.id;
    auto &module_id = module.id;
    auto &prod_map  = relationships->production_map;

    if (!prod_map.contains(ware_id)) {
        prod_map.emplace(ware_id, std::unordered_map<t_module_id, const Module *>{{module_id, &module}});
    } else {
        prod_map.at(ware_id).emplace(module_id, &module);
    }
}

void Data::registerModule(const Module &module) {
    auto &vector   = modules->modules;
    auto &map      = modules->module_map;
    auto &name_map = modules->module_name_map;

    // A module should not be registered twice
    if (map.contains(module.id)) {
        throw std::logic_error("module already registered: " + module.id.raw());
    }

    const auto &item    = vector.emplace_back(module);
    map[item.id]        = &item;
    name_map[item.name] = &item;

    if (!module.type.has_value())
        return;

    auto const &type  = module.type.value();
    auto const &macro = module.macro;
    // Assign module to specific maps based on its type and macro
    if (type == "storage") {
        modules->container_map[item.id] = &item;
        if (macro.contains("solid")) {
            modules->container_solid_map[item.id] = &item;
        } else if (macro.contains("liquid")) {
            modules->container_liquid_map[item.id] = &item;
        } else if (macro.contains("container")) {
            modules->container_container_map[item.id] = &item;
        } else {
            throw std::logic_error("unknown container type: " + macro);
        }
    } else if (type == "production") {
        modules->production_map[item.id] = &item;
    } else if (type == "processing") {
        modules->processing_map[item.id] = &item;
    } else if (type == "dockarea") {
        modules->dock_map[item.id] = &item;
    } else if (type == "pier") {
        modules->pierr_map[item.id] = &item;
    } else if (type == "habitation") {
        modules->habitation_map[item.id] = &item;
    } else if (type == "claim") {
        modules->claim_map[item.id] = &item;
    } else if (type == "defence") {
        modules->defence_map[item.id] = &item;
    } else if (type == "build") {
        modules->build_map[item.id] = &item;
    } else if (type == "connection") {
        modules->connection_map[item.id] = &item;
    } else if (type == "equip") {
        modules->equip_map[item.id] = &item;
    } else if (type == "venturer") {
        modules->venture_map[item.id] = &item;
    } else {
        spdlog::error("unknown type: {}", type);
        throw std::logic_error("unknown module type: " + type);
    }

    // Register all wares produced by this module
    for (auto &ware: item.production) {
        Data::registerWare(ware);
    }
}

void Data::registerWorkforce(const std::map<t_race_id, std::map<t_ware_id, double> > &workforce) {
    Data::workforce->consumption_map = workforce;
}

bool Data::isWareProduced(t_ware_id ware) {
    return relationships->production_map.contains(ware);
}

std::vector<WareAmount> Data::getWorkforceUsage(t_race_id race, unsigned int workforce_amount) {
    try {
        std::vector<WareAmount> ware_amounts{};
        auto                    pair_data = Data::workforce->consumption_map.at(race);

        for (const auto &[ware, consumption]: pair_data) {
            long int final_consumption = std::ceil(consumption * workforce_amount);
            ware_amounts.emplace_back(ware, final_consumption);
        }

        return ware_amounts;
    } catch (const std::out_of_range &e) {
        spdlog::error("could not find production method {}", race);
        throw;
    }
}
