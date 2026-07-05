//
// Created by pierre on 6/25/26.
//

#ifndef X4STATIONGENERATOR__UTILS_HPP
#define X4STATIONGENERATOR__UTILS_HPP
#include <filesystem>
#include <mutex>
#include <spdlog/fmt/bundled/format.h>

std::string to_winepath(const std::filesystem::path &path);

template<>
struct fmt::formatter<std::filesystem::path> {
    constexpr auto parse(format_parse_context &ctx) -> format_parse_context::iterator {
        return ctx.begin();
    }

    format_context::iterator format(const std::filesystem::path &path, fmt::format_context &ctx) const {
#ifdef WIN32
        auto const value = path.string();
#else
        auto const value = to_winepath(path);
#endif

        return fmt::format_to(ctx.out(), "{}", value);
    }
};


template<typename... Args>
std::string generate_command(Args... args) {
    std::string command{(... + fmt::format("'{}' ", args))};

#ifdef WIN32
    return command;
#else
    return fmt::format("wine {}", command);
#endif
}

template<typename T, typename Ret, typename... Args>
concept is_executable = requires(T t)
{
    { t(std::declval<Args>()...) } -> std::convertible_to<Ret>;
};

#endif // X4STATIONGENERATOR__UTILS_HPP
