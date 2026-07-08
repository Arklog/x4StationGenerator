//
// Created by pierre on 7/4/26.
//

#include "LangFile.hpp"

#include <regex>
#include <sstream>

namespace extractor {
    LangFile::LangFile(models::T &&t_file) {
        for (auto &page: t_file.page) {
            auto              page_id = std::move(page.id.value());
            std::stringstream ss;

            for (auto &translation: page.t) {
                auto t_id = std::move(translation.id.value());

                ss << page_id;
                ss << ',';
                ss << t_id;

                std::string key;
                ss >> key;
                ss.clear();

                _translations.emplace(std::move(key), std::move(translation.xml_content));
            }
        }
    }

    void LangFile::translate(std::string &value) const {
        const std::regex regex{"\\{([0-9]+,[0-9]+)\\}"};
        std::smatch      match_results;

        while (std::regex_search(value, match_results, regex)) {
            auto match = match_results[1];

            auto key         = match.str();
            auto translation = _translations.contains(key) ? _translations.at(key) : "[Translation not Found]";
            value.replace(match.first - 1, match.second + 1, translation);
        }
    }
} // namespace extractor
