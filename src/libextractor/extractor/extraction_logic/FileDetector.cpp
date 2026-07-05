//
// Created by pierre on 7/5/26.
//

#include "FileDetector.hpp"
#include "Archive.hpp"
#include "Extension.hpp"

extractor::FileDetector::FileDetector(const std::filesystem::path &x4_root, const std::filesystem::path &output_dir,
                                      const std::filesystem::path &XRCatTool) : archives{}, extensions{} {
    detect_main_archives(x4_root, output_dir, XRCatTool);
    detect_extensions(x4_root, output_dir, XRCatTool);
}

void extractor::FileDetector::detect_main_archives(const std::filesystem::path &x4_root,
                                                   const std::filesystem::path &output_dir,
                                                   const std::filesystem::path &XRCatTool) {
    auto it = std::filesystem::directory_iterator(x4_root);

    for (const auto &item: it) {
        if (item.is_regular_file() && item.path().extension() == ".cat" && !item.path().filename().string().
            contains("sig")) {
            archives.push_back(new Archive(item.path(), output_dir, XRCatTool));
        }
    }
}

void extractor::FileDetector::detect_extensions(const std::filesystem::path &x4_root,
                                                const std::filesystem::path &output_dir,
                                                const std::filesystem::path &XRCatTool) {
    auto x4_extension_dir = x4_root / "extensions";
    auto it               = std::filesystem::directory_iterator(x4_extension_dir);

    for (const auto &item: it) {
        if (!item.is_directory())
            continue;
        extensions.push_back(new Extension(item.path(), output_dir, XRCatTool));
    }
}

