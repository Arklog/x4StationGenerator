//
// Created by pierre on 7/7/26.
//

#ifndef X4STATIONGENERATOR__WARES_HPP
#define X4STATIONGENERATOR__WARES_HPP
#include <string>
#include <rfl/cli.hpp>

namespace extractor {
    class LangFile;
}

namespace extractor::models {
    struct Wares {
        struct Production {
            struct ProductionMethod {
                struct ProductionMethodDefault {
                    rfl::Attribute<std::string> race;
                };

                rfl::Attribute<std::string>                                     id;
                rfl::DefaultVal<rfl::Attribute<std::string> >                   name;
                rfl::Rename<"default", std::optional<ProductionMethodDefault> > default_;
            };

            std::vector<ProductionMethod> method;
        };

        struct Ware {
            struct WarePrice {
                rfl::Attribute<size_t> min;
                rfl::Attribute<size_t> average;
                rfl::Attribute<size_t> max;
            };

            struct Production {
                struct Primary {
                    struct Ware {
                        rfl::Attribute<std::string> ware;
                        rfl::Attribute<size_t>      amount;
                    };

                    std::vector<Ware> ware;
                };

                struct Effects {
                    struct Effect {
                        rfl::Attribute<std::string> type;
                        rfl::Attribute<double>      product;
                    };

                    std::vector<Effect> effect;
                };

                rfl::Attribute<double>                        time;
                rfl::Attribute<size_t>                        amount;
                rfl::Attribute<std::string>                   method;
                rfl::DefaultVal<rfl::Attribute<std::string> > name;
                std::optional<Primary>                        primary;
                rfl::DefaultVal<Effects>                      effects;
            };

            struct Component {
                rfl::Attribute<std::string> ref;
            };

            rfl::Attribute<std::string>                   id;
            rfl::Attribute<std::string>                   name;
            rfl::DefaultVal<rfl::Attribute<std::string> > group;
            rfl::Attribute<std::string>                   transport;
            rfl::Attribute<size_t>                        volume;
            WarePrice                                     price;
            std::vector<Production>                       production;
            std::optional<Component>                      component;
        };

        Production        production;
        std::vector<Ware> ware;
    };

    void translate(const extractor::LangFile &lang_file, Wares &wares);
} // extractor

#endif //X4STATIONGENERATOR__WARES_HPP
