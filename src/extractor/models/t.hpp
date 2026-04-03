
#pragma once

#include "t_xsd.hpp"

namespace t {

struct languageType;
struct languageType_page_t;
struct languageType_page_t_t_t;

struct languageType
{
    int64_t id;
    xsd::vector<xsd::any_attribute> other_attributes;
    xsd::vector<t::languageType_page_t> page;
};

struct languageType_page_t
{
    int64_t id;
    xsd::optional<xsd::string> title;
    xsd::optional<xsd::string> descr;
    xsd::optional<xsd::string> voice;
    xsd::vector<xsd::any_attribute> other_attributes;
    xsd::vector<t::languageType_page_t_t_t> t;
};

struct languageType_page_t_t_t : xsd::string
{
    int64_t id;
};

void load_file(const std::string& file, languageType& language);
void load_data(const std::string& data, languageType& language);

}
