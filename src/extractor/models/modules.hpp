
#pragma once

#include "modules_xsd.hpp"

namespace modules {

struct modulesType;
struct modulesType_module_t;
struct modulesType_module_t_category_t;
struct modulesType_module_t_category_t_extension_t;
struct modulesType_module_t_compatibilities_t;
struct modulesType_module_t_compatibilities_t_limits_t;
struct modulesType_module_t_compatibilities_t_maxlimits_t;
struct modulesType_module_t_compatibilities_t_production_t;

struct modulesType
{
    xsd::string noNamespaceSchemaLocation;
    xsd::vector<modules::modulesType_module_t> module;
};

struct modulesType_module_t_category_t
{
    xsd::optional<xsd::string> comment;
    xsd::optional<xsd::string> faction;
    xsd::optional<xsd::string> race;
    xsd::string tags;
    xsd::optional<xsd::string> ware;
    xsd::vector<modules::modulesType_module_t_category_t_extension_t> extension;
};

struct modulesType_module_t
{
    xsd::optional<xsd::string> base;
    xsd::string group;
    xsd::string id;
    modules::modulesType_module_t_category_t category;
    xsd::optional<modules::modulesType_module_t_compatibilities_t> compatibilities;
};

struct modulesType_module_t_category_t_extension_t
{
    xsd::string faction;
    xsd::optional<xsd::string> race;
};

struct modulesType_module_t_compatibilities_t
{
    xsd::optional<modules::modulesType_module_t_compatibilities_t_limits_t> limits;
    xsd::optional<modules::modulesType_module_t_compatibilities_t_maxlimits_t> maxlimits;
    xsd::vector<modules::modulesType_module_t_compatibilities_t_production_t> production;
};

struct modulesType_module_t_compatibilities_t_limits_t
{
    int64_t production;
};

struct modulesType_module_t_compatibilities_t_maxlimits_t
{
    int64_t production;
};

struct modulesType_module_t_compatibilities_t_production_t
{
    int64_t chance;
    xsd::optional<xsd::string> ref;
    xsd::optional<xsd::string> ware;
};

void load_file(const std::string& file, modulesType& modules);
void load_data(const std::string& data, modulesType& modules);

}
