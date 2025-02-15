//
// Created by pierre on 2/15/25.
//

#include "BuildSettings.hpp"

BuildSettings::BuildSettings() : _name("default")
{}

const std::string &BuildSettings::getName() const
{
    return _name;
}

void BuildSettings::setName(const std::string &name)
{
    _name = name;
}