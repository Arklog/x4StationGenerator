//
// Created by pierre on 7/5/26.
//

#ifndef X4STATIONGENERATOR__X4PATCHABLE_HPP
#define X4STATIONGENERATOR__X4PATCHABLE_HPP
#include <filesystem>
#include <deque>

#include "extractor/CacheFile.hpp"
#include "extractor/models/LangFile.hpp"

namespace extractor {
    struct Extension;

    struct X4Patchable {
        X4Patchable(const std::filesystem::path &in, const std::filesystem::path &out);

        virtual ~X4Patchable() = default;

        void patch(const std::filesystem::path & xml_diff_exec, LangFile &lang_file,
                   CacheFile<std::string, bool> &cache);

        std::filesystem::path in;
        std::filesystem::path out;
        bool                  output_exists;

        static void insert_patchable(const Extension *extension, std::vector<X4Patchable *> &patchables);

    private:
        void patch_move();

        void patch_diff(const std::filesystem::path &xml_diff_exec);

        void patch_lang(LangFile &lang_file);
    };
}

#endif //X4STATIONGENERATOR__X4PATCHABLE_HPP
