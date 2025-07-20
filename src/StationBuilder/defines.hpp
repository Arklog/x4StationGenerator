//
// Created by pierre on 7/20/25.
//

#ifndef DEFINES_HPP
#define DEFINES_HPP

#include "Data/Data.hpp"

struct WareTarget {
    t_ware_id ware_id;
    t_production_method_id production_method_id;
    unsigned int target;
};

typedef std::vector<const WareTarget*> t_target_list;
typedef std::vector<t_module_id> t_x4_complex;


#endif //DEFINES_HPP
