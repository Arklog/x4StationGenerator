//
// Created by pierre on 6/25/26.
//

#ifndef X4STATIONGENERATOR__UTILS_HPP
#define X4STATIONGENERATOR__UTILS_HPP
#include <filesystem>
#include <functional>
#include <mutex>
#include <thread>

std::string to_winepath (const std::filesystem::path &path);

template <typename T, typename Ret, typename... Args>
concept is_executable = requires (T t) {
    { t (std::declval<Args> ()...) } -> std::convertible_to<Ret>;
};

template <size_t NThreads, typename T, is_executable<void, T> FN>
class ThreadPool
{
    class ArgReserve
    {
      public:
	ArgReserve (std::vector<T> &&reserve) : _reserve (std::move (reserve))
	{}

	std::optional<T> get ()
	{
	    std::lock_guard lock (_mutex);

	    if (_reserve.empty ())
		return std::nullopt;

	    auto item = _reserve.back ();
	    _reserve.pop_back ();

	    return item;
	}

      private:
	std::vector<T> _reserve;
	std::mutex _mutex;
    };

  public:
    ThreadPool (FN fn, std::vector<T> &&args)
	: _args (std::move (args)), _fn{fn}, _running (false)
    {}

    ~ThreadPool ()
    {
	if (_running)
	    join ();
    }

    void start ()
    {
	if (_running)
	    throw std::runtime_error ("already running");

	_running = true;
	for (size_t i = 0; i < NThreads; ++i)
	{
	    _threads[i] = std::thread ([this] () {
		auto t = _args.get ();

		while (t.has_value ())
		{
		    _fn (t.value ());
		    t = _args.get ();
		}
	    });
	}
    }

    void join ()
    {
	if (!_running)
	    throw std::runtime_error ("not running");

	_running = false;
	for (size_t i = 0; i < NThreads; ++i)
	    _threads[i].join ();
    }

  private:
    ArgReserve _args;
    std::thread _threads[NThreads];
    FN _fn;
    bool _running;
};
#endif // X4STATIONGENERATOR__UTILS_HPP
