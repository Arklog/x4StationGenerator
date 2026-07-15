//
// Created by pierre on 7/5/26.
//

#ifndef X4STATIONGENERATOR__EXTENSION_HPP
#define X4STATIONGENERATOR__EXTENSION_HPP
#include "Archive.hpp"

namespace extractor {
    struct Extension : X4Extractable {
        Extension(const std::filesystem::path &path, const std::filesystem::path &output,
                  const std::filesystem::path &xrcattool);

        void extract(CacheFile<std::string, bool> &cache);

        std::filesystem::path output_tmp;
        std::vector<Archive>  archives;

    private:
        void extract_archives(CacheFile<std::string, bool> &cache);

        void extract_copy(CacheFile<std::string, bool> &cache);
    };
} // namespace extractor

#endif // X4STATIONGENERATOR__EXTENSION_HPP
