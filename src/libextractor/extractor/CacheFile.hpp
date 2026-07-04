//
// Created by pierre on 7/4/26.
//

#ifndef X4STATIONGENERATOR__CACHEFILE_HPP
#define X4STATIONGENERATOR__CACHEFILE_HPP

#include <filesystem>
#include <fstream>
#include <map>
#include <mutex>
#include <nlohmann/json.hpp>

namespace extractor {

    template <typename Key, typename Value> class CacheFile
    {
      public:
	using cache_type = std::map<Key, Value>;
	using key_type = Key;
	using value_type = Value;

	CacheFile (const std::filesystem::path &path)
	    : _cache_file_path (path), _cache (), _cache_lock (), _file_lock ()
	{
	    if (!std::filesystem::exists (_cache_file_path))
		return;

	    auto content = nlohmann::json::parse (std::ifstream (path));
	    nlohmann::from_json (content[0], _cache);
	}

	~CacheFile () { this->save (); }

	void register_entry (const key_type &key, const value_type &value)
	{
	    _cache_lock.lock ();
	    _cache[key] = value;
	    _cache_lock.unlock ();
	}

	void erase_entry (const key_type &key)
	{
	    _cache_lock.lock ();
	    _cache.erase (key);
	    _cache_lock.unlock ();
	}

	void save ()
	{
	    nlohmann::json content{_cache};

	    _file_lock.lock ();
	    std::ofstream file (_cache_file_path);
	    file << content.dump (0);
	    file.close ();
	    _file_lock.unlock ();
	}

	bool contains (const key_type &key)
	{
	    _cache_lock.lock ();
	    auto val = _cache.contains (key);
	    _cache_lock.unlock ();

	    return val;
	}

	value_type get (const key_type &key)
	{
	    _cache_lock.lock ();
	    auto val = _cache.at (key);
	    _cache_lock.unlock ();

	    return val;
	}

      private:
	std::filesystem::path _cache_file_path;
	cache_type _cache;
	std::mutex _cache_lock;
	std::mutex _file_lock;
    };
} // namespace extractor

#endif // X4STATIONGENERATOR__CACHEFILE_HPP
