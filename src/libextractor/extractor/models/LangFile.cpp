//
// Created by pierre on 7/4/26.
//

#include "LangFile.hpp"

#include <regex>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

namespace extractor {
    LangFile::LangFile(LangFile &&v) noexcept :
    _translations(std::move(v._translations)) {
    }

    LangFile &LangFile::operator=(LangFile &&v) noexcept {
        _translations = std::move(v._translations);
        return *this;
    }

    void LangFile::add_t(models::T &&t_file) {
        std::lock_guard lock(_translations_mutex);
        auto            csize = _translations.size();

        for (auto &page: t_file.page) {
            auto page_id = std::move(page.id.value());

            for (auto &translation: page.t) {
                auto t_id = std::move(translation.id.value());
                auto key  = fmt::format("{},{}", page_id, t_id);

                if (_translations.contains(key)) {
                    spdlog::error("Translation already exists for {}:\n - existing: {}\n - new : {}", key,
                                  _translations.at(key), translation.xml_content);
                }
                _translations.emplace(std::move(key), std::move(translation.xml_content));
            }
        }

        spdlog::info("Added {} translations: total {}", _translations.size() - csize, _translations.size());
    }

    void LangFile::translate(std::string &value) const {
        const std::regex regex{"\\{([0-9]+,[0-9]+)\\}"};
        const std::regex markup_reg{R"(\\033[A-Z])"};
        std::smatch      match_results;

        while (std::regex_search(value, match_results, regex)) {
            auto        match = match_results[1];
            auto        key   = match.str();
            std::string translation;

            if (!_translations.contains(key)) {
                spdlog::error("Translation not found for {}", match_results[0].str());
                translation = "[Translation not found]";
            } else {
                translation = _translations.at(key);
            }

            value.replace(match.first - 1, match.second + 1, translation);
        }

        while (std::regex_search(value, match_results, markup_reg)) {
            auto match = match_results[0];
            value.erase(match.first, match.second);
        }
    }
} // namespace extractor
