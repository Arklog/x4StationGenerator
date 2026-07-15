//
// Created by pierre on 7/5/26.
//
#include "extractor/extraction_logic/X4Extractable.hpp"

const std::vector<std::regex> extractor::X4Extractable::extraction_targets{
    std::regex{"libraries/wares.xml"},
    std::regex{"libraries/waregroups.xml"},
    // std::regex{"t/0001-l044.xml"},
    std::regex{"assets/structures/(?:dock|habitat|production|storage)/macros"},
};

extractor::X4Extractable::X4Extractable(const std::filesystem::path &file, const std::filesystem::path &output,
                                        const std::filesystem::path &x4RTool) :
path{file},
output{output},
x4RTool{x4RTool} {
}

