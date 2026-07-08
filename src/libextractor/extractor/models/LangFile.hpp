//
// Created by pierre on 7/4/26.
//

#ifndef X4STATIONGENERATOR__LANGFILE_HPP
#define X4STATIONGENERATOR__LANGFILE_HPP

#include <filesystem>
#include <unordered_map>
#include "extractor/models/T.hpp"

namespace extractor {
    class LangFile {
    public:
        LangFile(models::T &&t_file);

        void translate(std::string &value) const;

    private:
        std::unordered_map<std::string, std::string> _translations;
    };
} // namespace extractor

#endif // X4STATIONGENERATOR__LANGFILE_HPP
