//
// Created by pierre on 7/5/26.
//

#include "extractor/extraction_logic/Extension.hpp"

#include <spdlog/spdlog.h>

namespace extractor {
    Extension::Extension(const std::filesystem::path &path, const std::filesystem::path &output,
                         const std::filesystem::path &xrcattool) :
    X4Extractable(path, output, xrcattool) {
        output_tmp = output / "extensions" / path.filename();

        std::filesystem::directory_iterator it(path);
        for (auto &item: it) {
            auto item_path = item.path();

            if (is_regular_file(item_path) && item_path.extension() == ".cat" && !item_path.filename().string().
                contains("sig"))
                archives.emplace_back(Archive{item_path, output_tmp, xrcattool});
        }
    }

    void Extension::extract(CacheFile<std::string, bool> &cache) {
        if (cache.contains(path.string()) && cache.get(path.string())) {
            spdlog::info("Extension {} already extracted", path.string());
            return;
        }

        std::filesystem::create_directories(output);

        cache.register_entry(path.string(), false);
        cache.save();
        spdlog::info("Extracting extension {}", path.string());

        if (archives.empty())
            extract_copy(cache);
        else
            extract_archives(cache);

        cache.register_entry(path.string(), true);
        cache.save();
        spdlog::info("Extension {} extracted", path.string());
    }

    void Extension::extract_archives(CacheFile<std::string, bool> &cache) {
        for (auto &archive: archives)
            archive.extract(cache);
    }

    void Extension::extract_copy(CacheFile<std::string, bool> &cache) {
        std::filesystem::copy(path, output_tmp,
                              std::filesystem::copy_options::overwrite_existing |
                              std::filesystem::copy_options::recursive);
    }
} // extractor
