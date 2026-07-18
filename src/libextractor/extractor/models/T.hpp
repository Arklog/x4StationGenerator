//
// Created by pierre on 7/8/26.
//

#ifndef X4STATIONGENERATOR__T_HPP
#define X4STATIONGENERATOR__T_HPP
#include <rfl/cli.hpp>
#include <rfl/xml.hpp>

namespace extractor::models {
    struct T {
        struct Page {
            struct T {
                rfl::Attribute<std::string> id;
                std::string                 xml_content;
            };

            rfl::Attribute<std::string> id;
            std::vector<T>              t;
        };

        std::vector<Page> page;
    };

    struct TDiff {
        T add;
    };
} // extractor

#endif //X4STATIONGENERATOR__T_HPP
