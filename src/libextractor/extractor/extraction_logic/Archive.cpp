//
// Created by pierre on 7/5/26.
//

#include "extractor/extraction_logic/Archive.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/bundled/format.h>

#include "../utils.hpp"

namespace extractor {
    Archive::Archive(const std::filesystem::path &file, const std::filesystem::path &output,
                     const std::filesystem::path &x4RTool) : X4Extractable(file, output, x4RTool) {
    }

    Archive::Archive(const Archive &o) = default;

    void Archive::extract(CacheFile<std::string, bool> &cache) {
        if (cache.contains(path.string()) && cache.get(path.string())) {
            spdlog::info("Archive {} already extracted", path.string());
            return;
        }

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
} // namespace extractor
