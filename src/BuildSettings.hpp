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

    bool getWorkforce() const;

    [[nodiscard]] const StationSize &getSize() const;

    void setName(const std::string &name);

    void setWorkforce(bool workforce);

    void setSize(StationSize size);

private:
    std::string _name;
    bool        _workforce;
    StationSize _size;
};


#endif //X4STATIONGENERATOR_BUILDSETTINGS_HPP
