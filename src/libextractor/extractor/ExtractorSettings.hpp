//
// Created by pierre on 6/25/26.
//

#ifndef X4STATIONGENERATOR__EXTRACTORSETTINGS_HPP
#define X4STATIONGENERATOR__EXTRACTORSETTINGS_HPP

#include <filesystem>

namespace extractor {
    struct ExtractorSettings {
        std::filesystem::path XRCatToolPath;     // Path to XRCatTool executable
        std::filesystem::path XMLPatchPath;      // Path to XMLPatch path
        std::filesystem::path X4RootDirPath;     // Path to X4 game directory
        std::filesystem::path ExtractionDirPath; // Path to extraction dir
        std::filesystem::path OutputDirPath;     // Path to output dir
        std::size_t           nthreads;          // How many thread to use

        bool validate() const;
    };
} // namespace extractor

#endif // X4STATIONGENERATOR__EXTRACTORSETTINGS_HPP
