//
// Created by pierre on 2/15/25.
//

#ifndef X4STATIONGENERATOR_BUILDSETTINGS_HPP
#define X4STATIONGENERATOR_BUILDSETTINGS_HPP

#include <string>
#include "defines.hpp"

class BuildSettings {
public:
    BuildSettings();

    [[nodiscard]] const std::string &getName() const;

    [[nodiscard]] bool getWorkforce() const;

    [[nodiscard]] const StationSize &getSize() const;

    [[nodiscard]] size_t getSunFactor() const;

    void setName(const std::string &name);

    void setWorkforce(bool workforce);

    void setSize(StationSize size);

    void setSunFactor(size_t sun_factor);

private:
    std::string _name;
    bool        _workforce;
    StationSize _size;
    size_t      _sun_factor;
};


#endif //X4STATIONGENERATOR_BUILDSETTINGS_HPP
