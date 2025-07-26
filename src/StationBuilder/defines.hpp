//
// Created by pierre on 7/20/25.
//

#ifndef DEFINES_HPP
#define DEFINES_HPP

#include "Data/Data.hpp"

namespace ModuleType {
    extern const std::string pier;
    extern const std::string dock;
    extern const std::string storage;
}

struct WareTarget {
    t_ware_id ware_id;
    t_production_method_id production_method_id;
    long int prodution;
};

struct ModuleTarget {
    t_module_id module_id;
    size_t amount;

    bool operator==(const ModuleTarget &other) const;
    bool operator==(const t_module_id &module_id) const;
};

typedef std::vector<const WareTarget *> t_target_list;
typedef std::vector<t_module_id> t_x4_complex;
typedef std::vector<ModuleTarget> t_module_target_list;


#endif //DEFINES_HPP
