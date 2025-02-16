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

const StationSize &BuildSettings::getSize() const
{
    return _size;
}

void BuildSettings::setSize(StationSize size)
{
    _size = size;
}

bool BuildSettings::getWorkforce() const
{
    return _workforce;
}

void BuildSettings::setWorkforce(bool workforce)
{
    _workforce = workforce;
}

size_t BuildSettings::getSunFactor() const
{
    return _sun_factor;
}

void BuildSettings::setSunFactor(size_t sun_factor)
{
    _sun_factor = sun_factor;
}
