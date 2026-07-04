//
// Created by pierre on 7/4/26.
//

#ifndef X4STATIONGENERATOR__LANGFILE_HPP
#define X4STATIONGENERATOR__LANGFILE_HPP

#include "models/t.hpp"

#include <filesystem>
#include <unordered_map>

namespace extractor {
    class LangFile
    {
      public:
	LangFile (const std::filesystem::path &path);

	void translate (std::string &value);

      private:
	t::languageType _language;
	std::unordered_map<std::string, std::string> _translations;
    };

} // namespace extractor

#endif // X4STATIONGENERATOR__LANGFILE_HPP
