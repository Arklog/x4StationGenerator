//
// Created by pierre on 7/5/26.
//

#include "extractor/extraction_logic/Extension.hpp"

#include <spdlog/spdlog.h>

namespace extractor {
    Extension::Extension(const std::filesystem::path &path, const std::filesystem::path &output,
                         const std::filesystem::path &xrcattool) : X4Extractable(path, output, xrcattool) {
        output_tmp = output / "extensions" / path.filename();

        std::filesystem::directory_iterator it(path);
        for (auto &item: it) {
            auto item_path = item.path();

            if (is_regular_file(item_path) && item_path.extension() == ".cat" && !item_path.filename().string().
                contains("sig"))
                archive = Archive{item_path, output_tmp, xrcattool};
        }
    }

    void Extension::extract(CacheFile<std::string, bool> &cache) {
        std::filesystem::create_directories(output);

        if (archive.has_value()) {
            archive->extract(cache);
            return;
        }

        if (cache.contains(path.string()) && cache.get(path.string())) {
            spdlog::info("Extension {} already extracted", path.string());
            return;
        }

        cache.register_entry(path.string(), false);
        cache.save();
        spdlog::info("Extracting extension {}", path.string());

        std::filesystem::copy(path, output_tmp,
                              std::filesystem::copy_options::overwrite_existing |
                              std::filesystem::copy_options::recursive);

        cache.register_entry(path.string(), true);
        cache.save();
        spdlog::info("Extension {} extracted", path.string());
    }
} // extractor
