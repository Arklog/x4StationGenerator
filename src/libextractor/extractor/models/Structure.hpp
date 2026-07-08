//
// Created by pierre on 7/6/26.
//

#ifndef X4STATIONGENERATOR__STRUCTURE_HPP
#define X4STATIONGENERATOR__STRUCTURE_HPP

#include <rfl/cli.hpp>

#include "LangFile.hpp"

namespace extractor::models {
    struct Structure {
        struct StructureMacro {
            struct StructureProperties {
                struct StructureIdentification {
                    rfl::Attribute<std::string>                 name;
                    rfl::Attribute<std::optional<std::string> > makerrace;
                };

                struct StructureProduction {
                    struct StructureWareQueue {
                        rfl::Attribute<std::string> ware;
                        rfl::Attribute<std::string> method;
                    };

                    rfl::Attribute<std::string>     wares;
                    std::vector<StructureWareQueue> queue;
                };

                struct StructureWorkforce {
                    std::optional<rfl::Attribute<size_t> >      max;
                    std::optional<rfl::Attribute<size_t> >      capacity;
                    std::optional<rfl::Attribute<std::string> > race;
                };

                struct StructureCargo {
                    rfl::Attribute<size_t>      max;
                    rfl::Attribute<std::string> tags;
                };

                StructureIdentification            identification;
                std::optional<StructureProduction> production;
                std::optional<StructureWorkforce>  workforce;
                std::optional<StructureCargo>      cargo;
            };

            rfl::Attribute<std::string>                        name;
            rfl::Rename<"class", rfl::Attribute<std::string> > class_;
            StructureProperties                                properties;
        };

        StructureMacro macro;
    };

    void translate(const extractor::LangFile &lang_file, Structure &structure);
} // models

#endif //X4STATIONGENERATOR__STRUCTURE_HPP
