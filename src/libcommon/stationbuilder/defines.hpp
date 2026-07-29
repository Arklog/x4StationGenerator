//
// Created by pierre on 7/20/25.
//

#ifndef LIBCOMMON_DEFINES_HPP
#define LIBCOMMON_DEFINES_HPP

#include <string>
#include <vector>

#include "common/types/module/Module.hpp"
#include "utils/WareTargetContainer.hpp"

namespace common::types {
    struct StationSaveFile;
}

namespace common::stationbuilder {
    using t_module_id = types::module::Module::module_id;

    namespace ModuleType {
        extern const std::string pier;
        extern const std::string dock;
        extern const std::string storage;
        extern const std::string habitat;
    } // namespace ModuleType

    struct ModuleTarget {
        t_module_id module_id;
        size_t      amount;

        bool operator==(const ModuleTarget &other) const;

        bool operator==(const t_module_id &module_id) const;
    };

    typedef std::vector<const utils::WareTarget *> t_target_list;
    typedef std::vector<t_module_id>               t_x4_complex;
    typedef std::vector<ModuleTarget>              t_module_target_list;

    struct Settings {
        Settings() = default;

        explicit Settings(types::StationSaveFile &&save_file, const data::Store &store);

        std::string                name;              // name of the station
        double                     sunlight;          // sector sunlight
        t_module_id                workforce_module;  // habitat to use
        bool                       workforce_enables; // should the complex use workforce ?
        utils::WareTargetContainer ware_targets;
        t_module_target_list       docks;    // list of docks and pi
        t_module_target_list       storages; // list of storages

        bool operator==(const Settings &other) const;
    };
}
#endif // DEFINES_HPP
