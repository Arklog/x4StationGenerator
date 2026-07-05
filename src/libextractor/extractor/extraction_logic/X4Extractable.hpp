//
// Created by pierre on 7/5/26.
//

#ifndef X4STATIONGENERATOR__X4EXTRACTABLE_HPP
#define X4STATIONGENERATOR__X4EXTRACTABLE_HPP

#include <filesystem>

template<typename T, typename D>
class CacheFile;

namespace extractor {
    struct X4Extractable {
        std::filesystem::path                               path;
        std::filesystem::path                               output;
        std::reference_wrapper<const std::filesystem::path> x4RTool;

        X4Extractable(const std::filesystem::path &file, const std::filesystem::path &output,
                      const std::filesystem::path &x4RTool);

        virtual ~X4Extractable() = default;

        virtual void extract(CacheFile<std::string, bool> &) = 0;
    };
}

#endif //X4STATIONGENERATOR__X4EXTRACTABLE_HPP
