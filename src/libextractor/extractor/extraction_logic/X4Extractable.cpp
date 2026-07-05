//
// Created by pierre on 7/5/26.
//
#include "extractor/extraction_logic/X4Extractable.hpp"

extractor::X4Extractable::X4Extractable(const std::filesystem::path &file, const std::filesystem::path &output,
                                        const std::filesystem::path &x4RTool) : path{file}, output{output},
    x4RTool{x4RTool} {
}

