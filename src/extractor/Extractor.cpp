//
// Created by pierre on 6/25/26.
//

#include <filesystem>

#include "Extractor.hpp"

#include "CacheFile.hpp"
#include "LangFile.hpp"
#include "spdlog/spdlog.h"
#include "utils.hpp"

#include <fstream>
#include <queue>

namespace extractor {
    Extractor::Extractor (const ExtractorSettings &settings)
	: _settings (settings)
    {}

    void Extractor::extract () const
    {
	try
	{
	    if (!std::filesystem::exists (this->_settings.OutputDirPath))
		std::filesystem::create_directories (
		    this->_settings.OutputDirPath);

	    this->extract_main ();
	    this->extract_extension ();
	    this->patch_extension ();

	    LangFile test (_settings.OutputDirPath / "t/0001-l044.xml");
	}
	catch (const std::exception &e)
	{
	    spdlog::error ("{}", e.what ());
	}
    }

    void Extractor::extract_main () const
    {
	using namespace std::filesystem;

	spdlog::info ("[Extractor]: Extracting main archives");
	CacheFile<std::string, bool> cache_file (this->_settings.OutputDirPath
						 / "cache.json");

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

	std::mutex file_lock;
	auto const cache_file_path
	    = this->_settings.OutputDirPath / "cache.json";

	using param = path;
	std::vector<param> archives;

	for (auto &iter : dirs)
	{
	    if (!iter.is_regular_file () || iter.path ().extension () != ".cat"
		|| iter.path ().filename ().string ().contains ("sig"))
		continue;

	    auto path = iter.path ();
	    archives.push_back (path);
	}

	auto fn = [this, &file_lock, &cache_file] (const path &archive) {
	    if (cache_file.contains (archive.string ())
		&& cache_file.get (archive.string ()))
	    {
		spdlog::info ("[Extractor]: {} already extracted",
			      archive.string ());
		return;
	    }

	    cache_file.register_entry (archive.string (), false);
	    cache_file.save ();
	    spdlog::info ("[Extractor]: {} extracting", archive.string ());

#ifdef WIN32
	    auto command = fmt::format ("'{}' -in '{}' -out '{}'",
					_settings.XRCatToolPath.string (),
					archive.string (),
					_settings.OutputDirPath.string ());
#else
	    auto command = fmt::format ("wine '{}' -in '{}' -out '{}'",
					_settings.XRCatToolPath.string (),
					to_winepath (archive),
					to_winepath (_settings.OutputDirPath));
#endif

	    system (command.c_str ());
	    cache_file.register_entry (archive.string (), true);
	    cache_file.save ();
	    spdlog::info ("[Extractor]: {} extracted", archive.string ());
	};

	ThreadPool<5, param, decltype (fn)> threads (fn, std::move (archives));
	threads.start ();
    }

    void Extractor::extract_extension () const
    {
	using param = std::filesystem::path;
	spdlog::info ("[Extractor]: Extracting extension archives");

	auto extension_folder = _settings.X4RootDirPath / "extensions";
	CacheFile<std::string, bool> cache_file (this->_settings.OutputDirPath
						 / "cache.json");
	std::vector<param> extensions;

	auto dirs = std::filesystem::directory_iterator (extension_folder);
	for (auto &iter : dirs)
	{
	    if (!iter.is_directory ())
		continue;
	    extensions.push_back (iter.path ());
	}

	auto get_archive = [] (const std::filesystem::path &path)
	    -> std::optional<std::filesystem::path> {
	    auto dirs = std::filesystem::directory_iterator (path);
	    for (auto &iter : dirs)
	    {
		if (iter.is_regular_file ()
		    && iter.path ().extension () == ".cat")
		    return iter.path ();
	    }

	    return std::nullopt;
	};

	auto fn = [get_archive, this, &cache_file] (const param &path) {
	    auto archive = get_archive (path);
	    auto output = this->_settings.OutputDirPath / "extensions"
			  / path.filename ();
	    if (!archive)
		return;

	    std::filesystem::create_directories (output);

	    if (cache_file.contains (archive.value ().string ())
		&& cache_file.get (archive.value ().string ()))
	    {
		spdlog::info ("[Extractor]: {} already extracted",
			      path.string ());
		return;
	    }
	    cache_file.register_entry (archive.value ().string (), false);
	    cache_file.save ();
	    spdlog::info ("[Extractor]: {} extracting", path.string ());

#ifdef WIN32
	    auto command = fmt::format ("'{}' -in '{}' -out '{}'",
					this->_settings.XRCatToolPath.string (),
					archive->string (), output.string ());
#else
	    auto win_path = to_winepath (archive->string ());
	    auto win_output = to_winepath (output.string ());
	    auto win_exec
		= to_winepath (this->_settings.XRCatToolPath.string ());

	    auto command = fmt::format ("wine '{}' -in '{}' -out '{}'",
					win_exec, win_path, win_output);
#endif

	    system (command.c_str ());
	    cache_file.register_entry (archive.value ().string (), true);
	    cache_file.save ();
	    spdlog::info ("[Extractor]: {} extracted",
			  archive.value ().string ());
	};

	ThreadPool<5, param, decltype (fn)> pool (fn, std::move (extensions));
	pool.start ();
    }

    void Extractor::patch_extension () const
    {
	using namespace std::filesystem;
	struct path_info
	{
	    path diff_file;
	    path source_file;
	};
	using arg_type = std::vector<path_info>;
	using queue_type = std::unordered_map<path, arg_type>;

	auto extension_folder = _settings.OutputDirPath / "extensions";
	queue_type queue{};
	CacheFile<std::string, bool> cache_file (_settings.OutputDirPath
						 / "cache.json");

	auto iter = recursive_directory_iterator (extension_folder);
	for (auto &item : iter)
	{
	    if (!item.is_regular_file () || item.path ().extension () != ".xml")
		continue;

	    auto file_without_root
		= item.path ().lexically_relative (extension_folder);
	    auto tmp = file_without_root.string ();
	    tmp.erase (0, tmp.find ('/'));
	    file_without_root = tmp;

	    auto diff_file = iter->path ();
	    auto file_to_patch = path{_settings.OutputDirPath.string ()
				      + file_without_root.string ()};

	    if (!exists (file_to_patch))
	    {
		spdlog::info ("[Extractor]: {} not found, skipping",
			      file_to_patch.string ());
		continue;
	    }

	    queue[file_without_root].emplace_back (diff_file, file_to_patch);
	}

	auto fn = [this, &cache_file] (const arg_type &queue) {
	    for (const auto &[diff_file, file_to_patch] : queue)
	    {
		if (cache_file.contains (file_to_patch.string ())
		    && cache_file.get (file_to_patch.string ()))
		{
		    spdlog::info ("[Extractor]: {} already patched",
				  file_to_patch.string ());
		    continue;
		}

		cache_file.register_entry (file_to_patch.string (), false);
		spdlog::info ("[Extractor]: Patching {}",
			      file_to_patch.string ());

#ifdef WIN32
		auto command
		    = fmt::format ("'{}' -o '{}' -d '{}' -u '{}'",
				   _settings.XMLPatchPath.string (),
				   file_to_patch.string (), diff_file.string (),
				   file_to_patch.string ());
#else
		auto command
		    = fmt::format ("wine '{}' -o '{}' -d '{}' -u '{}'",
				   to_winepath (_settings.XMLPatchPath),
				   to_winepath (file_to_patch),
				   to_winepath (diff_file),
				   to_winepath (file_to_patch));
#endif

		system (command.c_str ());
		cache_file.register_entry (file_to_patch.string (), true);
		cache_file.save ();
		spdlog::info ("[Extractor]: {} patched",
			      file_to_patch.string ());
	    }
	};

	std::vector<arg_type> args;
	for (auto &[_, item] : queue)
	{
	    args.push_back (item);
	}
	ThreadPool<5, arg_type, decltype (fn)> pool (fn, std::move (args));
	pool.start ();
    }
} // namespace extractor