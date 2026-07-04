
#pragma once

#include "waregroups_xsd.hpp"

namespace waregroups {

struct groupsType;
struct groupsType_group_t;

struct groupsType
{
    xsd::vector<waregroups::groupsType_group_t> group;
};

struct groupsType_group_t
{
    xsd::optional<xsd::string> factoryhudicon;
    xsd::optional<xsd::string> factorymapicon;
    xsd::optional<xsd::string> factoryname;
    xsd::optional<xsd::string> icon;
    xsd::string id;
    xsd::string name;
    xsd::optional<int64_t> priority;
    xsd::string tags;
    xsd::optional<int64_t> tier;
};

void load_file(const std::string& file, groupsType& groups);
void load_data(const std::string& data, groupsType& groups);

}
