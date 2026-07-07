//
// Created by pierre on 7/6/26.
//

#ifndef X4STATIONGENERATOR__STRUCTURE_HPP
#define X4STATIONGENERATOR__STRUCTURE_HPP

#include <rfl/cli.hpp>

namespace structure {
    struct StructureWorkforce {
        std::optional<rfl::Attribute<size_t> >      max;
        std::optional<rfl::Attribute<size_t> >      capacity;
        std::optional<rfl::Attribute<std::string> > race;
    };

    struct StructureWareQueue {
        rfl::Attribute<std::string> ware;
        rfl::Attribute<std::string> method;
    };

    struct StructureProduction {
        rfl::Attribute<std::string> wares;
    };

    struct StructureIdentification {
        rfl::Attribute<std::string>                 name;
        rfl::Attribute<std::optional<std::string> > makerrace;
    };

    struct StructureCargo {
        rfl::Attribute<size_t>      max;
        rfl::Attribute<std::string> tags;
    };

    struct StructureProperties {
        StructureIdentification            identification;
        std::optional<StructureProduction> production;
        std::optional<StructureWorkforce>  workforce;
        std::optional<StructureCargo>      cargo;
    };

    struct StructureMacro {
        rfl::Attribute<std::string>                        name;
        rfl::Rename<"class", rfl::Attribute<std::string> > class_;
        StructureProperties                                properties;
    };

    struct Structure {
        StructureMacro macro;
    };
} // structure

#endif //X4STATIONGENERATOR__STRUCTURE_HPP
