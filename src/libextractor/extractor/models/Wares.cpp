//
// Created by pierre on 7/7/26.
//

#include "Wares.hpp"

#include "extractor/LangFile.hpp"

namespace extractor::models::wares {
} // extractor

void extractor::models::translate(const extractor::LangFile &lang_file, Wares &wares) {
    for (auto &production_method: wares.production.method) {
        lang_file.translate(production_method.name.value().value());
    }

    for (auto &ware: wares.ware) {
        lang_file.translate(ware.name.value());

        for (auto &production: ware.production) {
            lang_file.translate(production.name.value().value());
        }
    }
}
