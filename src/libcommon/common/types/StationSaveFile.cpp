//
// Created by pierre on 7/24/26.
//
#include "common/types/StationSaveFile.hpp"

#include "stationbuilder/Complex.hpp"

void common::types::StationSaveFile::fromComplex(const stationbuilder::Complex &complex, const data::Store &store,
                                                 StationSaveFile &              save_file) {
    if (complex.name.empty())
        throw std::runtime_error{"Station is missing a name"};
    if (complex.habitat_id.empty())
        throw std::runtime_error{"Invalid habitat id"};

    auto add_target = [&](const stationbuilder::WareTarget *target) {
        stationbuilder::WareTarget target_{
            target->ware_id,
            target->source_module,
            target->prodution,
            target->is_secondary
        };

        if (target_.is_secondary)
            save_file.secondary_targets.emplace_back(std::move(target_));
        else
            save_file.primary_targets.emplace_back(std::move(target_));
    };
    auto add_module = [&](const stationbuilder::t_x4_complex::value_type &module_id) {
        auto  module      = store.modules.by_id.at(module_id);
        auto &module_type = module->module.get().module_type;

        if (module_type == module::ModuleType::pier || module_type == module::ModuleType::dock)
            save_file.docks[module_id]++;
        else if (module_type == module::ModuleType::storage)
            save_file.storages[module_id]++;
    };


    save_file.name       = complex.name;
    save_file.sun        = complex.sun;
    save_file.workforce  = complex.workforce;
    save_file.habitat_id = complex.habitat_id;

    save_file.docks             = {};
    save_file.storages          = {};
    save_file.primary_targets   = {};
    save_file.secondary_targets = {};

    std::ranges::for_each(complex.wares.getPrimaryAndSecondaryTargets(), add_target);
    std::ranges::for_each(complex.complex, add_module);
}
