//
// Created by pierre on 6/25/26.
//

#ifndef X4STATIONGENERATOR__EXTRACTOR_HPP
#define X4STATIONGENERATOR__EXTRACTOR_HPP

#include "CacheFile.hpp"
#include "ExtractorSettings.hpp"
#include "ThreadPool.hpp"
#include "utils.hpp"

#include <spdlog/spdlog.h>

#include "common/types/Workforce.hpp"
#include "models/LangFile.hpp"

namespace extractor {
    /**
     * @class Extractor
     * @brief Provides functionality to extract resources using the
     * configuration provided in ExtractorSettings.
     *
     * The Extractor class is designed to handle resource extraction tasks. It
     * uses the settings specified in an instance of ExtractorSettings to
     * configure paths and other necessary parameters.
     */
    class Extractor {
    public:
        Extractor(const ExtractorSettings &settings);

        /**
         * Extract all x4 files and patch them
         */
        void extract();

        /**
         * Read extracted files to build usable data models
         */
        void parse();

    private:
        ExtractorSettings _settings;

        /**
         * Extract main x4 files
         */
        void extract_main() const;

        /**
         * Extract mods and extension
         */
        void extract_extension() const;

        /**
         * Patch main x4 files with extension ones
         */
        void patch_extension() const;

        template<typename T>
        void save_datas(const std::string &subfolder, std::vector<T> &datas, common::ThreadPool &pool) const {
            for (auto &i: datas) {
                auto fn = [this, subfolder, i]() {
                    auto path = _settings.OutputDirPath / subfolder;
                    std::filesystem::create_directories(path);
                    path /= fmt::format("{}.json", i.module.value().id);
                    rfl::json::save(path.string(), i);
                };
                pool.enqueue(new common::Task(fn));
            }
            spdlog::info("Saved {} {}", datas.size(), subfolder);
        }

        LangFile lang_file;
    };
} // namespace extractor

#endif // X4STATIONGENERATOR__EXTRACTOR_HPP
