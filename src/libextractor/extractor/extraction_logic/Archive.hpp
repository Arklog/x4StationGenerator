//
// Created by pierre on 7/5/26.
//

#ifndef X4STATIONGENERATOR__ARCHIVE_HPP
#define X4STATIONGENERATOR__ARCHIVE_HPP
#include "extractor/CacheFile.hpp"

#include <filesystem>

#include "X4Extractable.hpp"

namespace extractor {
    struct Archive : public X4Extractable {
        Archive(const std::filesystem::path &path,
                const std::filesystem::path &output,
                const std::filesystem::path &x4RTool);

        Archive(const Archive &);

        void extract(CacheFile<std::string, bool> &cache) override;
    };
} // namespace extractor

#endif // X4STATIONGENERATOR__ARCHIVE_HPP
