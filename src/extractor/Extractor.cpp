//
// Created by pierre on 6/25/26.
//

#include <filesystem>

#include "Extractor.hpp"

#include "spdlog/spdlog.h"
#include "utils.hpp"

namespace extractor {
    Extractor::Extractor (const ExtractorSettings &settings)
	: _settings (settings)
    {}

    void Extractor::extract () const
    {
	try
	{
	    this->extract_main ();
	    // this->extract_extension ();
	}
	catch (const std::exception &e)
	{
	    spdlog::error ("{}", e.what ());
	}
    }

    void Extractor::extract_main () const
    {
	using namespace std::filesystem;

	// check input and output dir
	if (!exists (this->_settings.X4RootDirPath))
	    throw std::runtime_error (
		fmt::format ("[Extractor]: '{}' does not exist",
			     this->_settings.X4RootDirPath.string ()));
	if (!exists (this->_settings.OutputDirPath))
	    throw std::runtime_error (
		fmt::format ("[Extractor]: '{}' does not exist",
			     this->_settings.OutputDirPath.string ()));

	// iterate archive and extract
	auto dirs = std::filesystem::directory_iterator (
	    this->_settings.X4RootDirPath);

	std::vector<std::string> commands;
	for (auto &iter : dirs)
	{
	    if (!iter.is_regular_file () || iter.path ().extension () != ".cat")
		continue;

	    auto path = iter.path ();
	    commands.push_back (
		fmt::format ("wine '{}' -in '{}' -out '{}'",
			     _settings.XRCatToolPath.string (),
			     to_winepath (path),
			     to_winepath (_settings.OutputDirPath)));
	}

	ThreadPool<20, std::string, void (*) (std::string)> threads (
	    [] (std::string command) { system (command.c_str ()); },
	    std::move (commands));
	threads.start ();
    }
} // namespace extractor