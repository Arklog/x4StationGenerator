//
// Created by pierre on 7/8/26.
//

#include "Waregroups.hpp"

namespace extractor::models {
    void translate(const extractor::LangFile &lang_file, Waregroups &waregroups) {
        for (auto &i: waregroups.group)
            translate(lang_file, i);
    }

    void translate(const extractor::LangFile &lang_file, Waregroups::Group &waregroup) {
        lang_file.translate(waregroup.name.value());
    }
}
