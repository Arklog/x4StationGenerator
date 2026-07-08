//
// Created by pierre on 7/6/26.
//

#include "Structure.hpp"

namespace extractor::models {
    void translate(const extractor::LangFile &lang_file, Structure &structure) {
        lang_file.translate(structure.macro.name.value());
        lang_file.translate(structure.macro.properties.identification.name.value());
    }
} // models
