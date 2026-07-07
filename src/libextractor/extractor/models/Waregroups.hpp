//
// Created by pierre on 7/8/26.
//

#ifndef X4STATIONGENERATOR__WAREGROUPS_HPP
#define X4STATIONGENERATOR__WAREGROUPS_HPP

#include <rfl/cli.hpp>

namespace extractor::models {
    struct Waregroups {
        struct Group {
            rfl::Attribute<std::string>              id;
            rfl::Attribute<std::string>              name;
            rfl::DefaultVal<rfl::Attribute<size_t> > tier;
        };

        std::vector<Group> group;
    };
} // extractor

#endif //X4STATIONGENERATOR__WAREGROUPS_HPP
