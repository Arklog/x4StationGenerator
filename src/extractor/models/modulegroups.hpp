
#pragma once

#include "modulegroups_xsd.hpp"

namespace modulegroups {

struct groupsType;
struct groupsType_group_t;
struct groupsType_group_t_select_t;

struct groupsType
{
    xsd::vector<modulegroups::groupsType_group_t> group;
};

struct groupsType_group_t
{
    xsd::string name;
    xsd::vector<modulegroups::groupsType_group_t_select_t> select;
};

struct groupsType_group_t_select_t
{
    xsd::string macro;
};

void load_file(const std::string& file, groupsType& groups);
void load_data(const std::string& data, groupsType& groups);

}
