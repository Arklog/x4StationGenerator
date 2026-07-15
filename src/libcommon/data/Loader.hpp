//
// Created by pierre on 7/15/25.
//

#ifndef LOADER_H
#define LOADER_H

#include "Store.hpp"
#include "nlohmann/json.hpp"
#include <fmt/format.h>
#include <rfl/json.hpp>

namespace common::data {
    class Loader {
    public:
        Loader(Store &store, std::filesystem::path path);

        void load();

        Store &_store;

    private:
        std::filesystem::path _path;

        template<typename T>
        void load_datas_from(const std::string &path) {
            auto const datapath = _path / path;
            if (!std::filesystem::exists(datapath) || !std::filesystem::is_directory(datapath))
                throw std::runtime_error(fmt::format("Directory {} does not exist", datapath.string()));

            auto it = std::filesystem::directory_iterator(datapath);
            for (const auto &item: it) {
                if (!item.is_regular_file() || item.path().extension() != ".json")
                    continue;

                auto data = rfl::json::load<T>(item.path());
                if (data.has_value())
                    _store.add(std::move(data.value()));
            }
        }
    };
}
#endif // LOADER_H
