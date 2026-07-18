//
// Created by pierre on 7/5/26.
//

#include "extractor/extraction_logic/Archive.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/bundled/format.h>

#include "../utils.hpp"

namespace extractor {
    Archive::Archive(const std::filesystem::path &file, const std::filesystem::path &output,
                     const std::filesystem::path &x4RTool) :
    X4Extractable(file, output, x4RTool) {
    }

    Archive::Archive(const Archive &o) = default;

    void Archive::extract(CacheFile<std::string, bool> &cache) {
        if (cache.contains(path.string()) && cache.get(path.string())) {
            spdlog::info("Archive {} already extracted", path.string());
            return;
        }

        // archive does not contain valid files, cache it and exit
        if (!validate()) {
            spdlog::info("Archive {} does not contain valid files, skipping", path.string());
            cache.register_entry(path.string(), true);
            cache.save();
            return;
        }

        // extract archive content and add to cache
        cache.register_entry(path.string(), false);
        cache.save();

        spdlog::info("Extracting archive {}", path.string());
        if (!std::filesystem::exists(output)) {
            spdlog::warn("Directory {} does not exist, creating it", output.string());
            std::filesystem::create_directories(output);
        }

        auto command = generate_command(x4RTool.get(), "-in", path, "-out", output);
        system(command.c_str());
        cache.register_entry(path.string(), true);
        cache.save();

        spdlog::info("Archive {} extracted", path.string());
    }

    bool Archive::validate() {
        load_cat_content();

        auto ret = std::ranges::any_of(extraction_targets, [this](const auto &target) -> bool {
            return std::regex_search(this->cat_content, target);
        });

        if (!ret) {
            spdlog::info("Archive {} does not contain valid files, skipping", path.string());
        }

        return ret;
    }

    bool Archive::load_cat_content() {
        if (!cat_content.empty())
            return true;

        auto cat_file = path;
        cat_file.replace_extension(".cat");

        if (!std::filesystem::exists(cat_file)) {
            spdlog::error("Archive {} does not exist", path.string());
            return false;
        }

        std::ifstream     cat_stream(cat_file);
        std::stringstream buffer;
        buffer << cat_stream.rdbuf();
        cat_content = buffer.str();

        return true;
    }

    BaseGameArchive::BaseGameArchive(const std::filesystem::path &path, const std::filesystem::path &output,
                                     const std::filesystem::path &x4RTool) :
    Archive(path, output, x4RTool) {
    }

    bool BaseGameArchive::validate() {
        auto       res = Archive::validate();
        std::regex reg{"t/0001-l044.xml"};
        return std::regex_search(cat_content, reg) || res;
    }
} // namespace extractor
