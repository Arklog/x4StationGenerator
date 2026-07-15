//
// Created by pierre on 7/5/26.
//

#ifndef X4STATIONGENERATOR__X4EXTRACTABLE_HPP
#define X4STATIONGENERATOR__X4EXTRACTABLE_HPP

#include <filesystem>
#include <regex>
#include <vector>

template<typename T, typename D>
class CacheFile;

namespace extractor {
    struct X4Extractable {
        static const std::vector<std::regex> extraction_targets; // list of files or directory we want to extract

        std::filesystem::path                               path;    // path of the file or folder
        std::filesystem::path                               output;  // output directory
        std::reference_wrapper<const std::filesystem::path> x4RTool; // path to XRCatTool

        X4Extractable(const std::filesystem::path &file, const std::filesystem::path &output,
                      const std::filesystem::path &x4RTool);

        virtual ~X4Extractable() = default;

        virtual void extract(CacheFile<std::string, bool> &) = 0;
    };
}

#endif //X4STATIONGENERATOR__X4EXTRACTABLE_HPP
