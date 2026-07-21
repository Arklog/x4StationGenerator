//
// Created by pierre on 7/20/25.
//

#ifndef LIBCOMMON_DEFINES_HPP
#define LIBCOMMON_DEFINES_HPP

#include <string>
#include <vector>

#include "common/types/Ware.hpp"
#include "common/types/module/Module.hpp"

namespace common::stationbuilder {
    using t_module_id = types::module::Module::module_id;

    namespace ModuleType {
        extern const std::string pier;
        extern const std::string dock;
        extern const std::string storage;
        extern const std::string habitat;
    } // namespace ModuleType

    struct WareTarget {
        types::Ware::ware_id             ware_id;
        types::module::Module::module_id source_module;
        long int                         prodution;
        bool                             is_secondary;
    };

    struct ModuleTarget {
        t_module_id module_id;
        size_t      amount;

        bool operator==(const ModuleTarget &other) const;

        bool operator==(const t_module_id &module_id) const;
    };

    struct Settings {
        std::string name;
        double      sunlight;
        t_module_id workforce_module;
        bool        workforce_enables;
    };

    typedef std::vector<const WareTarget *> t_target_list;
    typedef std::vector<t_module_id>        t_x4_complex;
    typedef std::vector<ModuleTarget>       t_module_target_list;
}
#endif // DEFINES_HPP
