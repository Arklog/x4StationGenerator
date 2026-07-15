//
// Created by pierre on 6/25/26.
//

#include "utils.hpp"

#include "spdlog/fmt/bundled/format.h"

std::string to_winepath(const std::filesystem::path &path) {
#ifdef WIN32
    return path.string();
#else
    char output[1024];
    auto command = fmt::format("winepath -w '{}'", path.string());
    auto pipe    = popen(command.c_str(), "r");

    if (!pipe) {
        auto error = strerror(errno);
        throw std::runtime_error(
            fmt::format("popen failed: {} ({})", error, command));
    }

    if (!fgets(output, sizeof (output), pipe))
        throw std::runtime_error("fgets failed");

    pclose(pipe);
    auto winepath = std::string(output);
    winepath.pop_back();
    return winepath;
#endif
}

std::string str(char *v) {
    return std::string(v);
}
