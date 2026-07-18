//
// Created by pierre on 7/5/26.
//

#ifndef X4STATIONGENERATOR__FILEDETECTOR_HPP
#define X4STATIONGENERATOR__FILEDETECTOR_HPP

#include <filesystem>
#include <vector>

namespace extractor {
    struct Archive;
    struct Extension;

    struct FileDetector {
        FileDetector(const std::filesystem::path &x4_root, const std::filesystem::path &output_dir,
                     const std::filesystem::path &XRCatTool);

        ~FileDetector();

        std::vector<Archive *>   archives;   // base games archives
        std::vector<Extension *> extensions; // mods and dlc

    private:
        void detect_main_archives(const std::filesystem::path &x4_root, const std::filesystem::path &output_dir,
                                  const std::filesystem::path &XRCatTool);

        void detect_extensions(const std::filesystem::path &x4_root, const std::filesystem::path &output_dir,
                               const std::filesystem::path &XRCatTool);
    };
}
#endif //X4STATIONGENERATOR__FILEDETECTOR_HPP
