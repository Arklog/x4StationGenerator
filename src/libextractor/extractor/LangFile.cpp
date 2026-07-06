//
// Created by pierre on 7/4/26.
//

#include "LangFile.hpp"

#include <fstream>
#include <regex>
#include <sstream>

namespace extractor {
    LangFile::LangFile(const std::filesystem::path &path) {
        t::languageType language;
        t::load_file(path.string(), language);

        for (const auto &page: language.page) {
            auto              page_id = page.id;
            std::stringstream ss;

            for (const auto &translation: page.t) {
                auto t_id = translation.id;

                ss << page_id;
                ss << '-';
                ss << t_id;

                std::string key;
                ss >> key;

                _translations.emplace(key, translation);
            }
        }
    }

    void LangFile::translate(std::string &value) const {
        const std::regex regex{"{(\\d+-\\d+)}"};
        std::smatch      match_results;

        while (std::regex_search(value, match_results, regex)) {
            auto match = match_results[1];

            auto key         = match.str();
            auto translation = _translations.at(key);
            value.replace(match.first - 1, match.second + 1, translation);
        }
    }
} // namespace extractor
