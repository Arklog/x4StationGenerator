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
    auto &map      = wares->ware_map;
    auto &name_map = wares->ware_name_map;

    // A ware should not be registered twice
    if (map.contains(ware.id)) {
        spdlog::info("ware {} already registered", ware.id);
        return;
    }

    map[ware.id]        = &ware;
    name_map[ware.name] = &ware;

    Data::registerWareGroup(ware.group);
}

void Data::registerWareGroup(const WareGroup &ware_group) {
    auto &map = ware_groups->ware_group_map;

    // A ware group should not be registered twice
    if (map.contains(ware_group.id)) {
        spdlog::info("ware group {} already registered", ware_group.id);
        return;
    }

    map[ware_group.id] = &ware_group;
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
    auto &vector = modules->modules;

    vector.emplace_back(module);
}

void Data::processModule(const Module &module) {
    modules->module_map[module.id]        = &module;
    modules->module_name_map[module.name] = &module;

    if (!module.type.has_value())
        return;

    auto const &type  = module.type.value();
    auto const &macro = module.macro;
    // Assign module to specific maps based on its type and macro
    if (type == "storage") {
        modules->container_map[module.id] = &module;
        if (macro.contains("solid")) {
            modules->container_solid_map[module.id] = &module;
        } else if (macro.contains("liquid")) {
            modules->container_liquid_map[module.id] = &module;
        } else if (macro.contains("container")) {
            modules->container_container_map[module.id] = &module;
        } else {
            // throw std::logic_error("unknown container type: " + macro);
        }
    } else if (type == "production") {
        modules->production_map[module.id] = &module;
    } else if (type == "processing") {
        modules->processing_map[module.id] = &module;
    } else if (type == "dockarea") {
        modules->dock_map[module.id] = &module;
    } else if (type == "pier") {
        modules->pierr_map[module.id] = &module;
    } else if (type == "habitation") {
        modules->habitation_map[module.id] = &module;
    } else if (type == "claim") {
        modules->claim_map[module.id] = &module;
    } else if (type == "defence") {
        modules->defence_map[module.id] = &module;
    } else if (type == "build") {
        modules->build_map[module.id] = &module;
    } else if (type == "connection") {
        modules->connection_map[module.id] = &module;
    } else if (type == "equip") {
        modules->equip_map[module.id] = &module;
    } else if (type == "venturer") {
        modules->venture_map[module.id] = &module;
    } else {
        spdlog::error("unknown type: {}", type);
        throw std::logic_error("unknown module type: " + type);
    }

    // Register all wares produced by this module
    for (auto &ware: module.production) {
        registerWare(ware);
        registerRelationship(module, ware);
    }
}

void Data::registerWorkforce(const std::map<t_race_id, std::map<t_ware_id, double> > &workforce) {
    Data::workforce->consumption_map = workforce;
}

void Data::processData() {
    for (const auto &module: modules->modules)
        processModule(module);
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
