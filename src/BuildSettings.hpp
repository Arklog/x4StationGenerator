//
// Created by pierre on 2/15/25.
//

#ifndef X4STATIONGENERATOR_BUILDSETTINGS_HPP
#define X4STATIONGENERATOR_BUILDSETTINGS_HPP

#include <string>

class BuildSettings {
public:
    BuildSettings();

    const std::string &getName() const;

    void setName(const std::string &name);

private:
    std::string _name;
};


#endif //X4STATIONGENERATOR_BUILDSETTINGS_HPP
