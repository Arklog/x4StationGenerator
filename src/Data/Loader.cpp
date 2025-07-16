//
// Created by pierre on 7/15/25.
//

#include "Loader.h"

#include <fstream>
#include <iostream>
#include <qdirlisting.h>
#include <qstring.h>

#include "spdlog/spdlog.h"
#include "nlohmann/json.hpp"

void Loader::_load_wares() {
    spdlog::info("Loading wares");

    for (const auto &file_iterator: QDirListing(QString("assets/wares/"), QDirListing::IteratorFlag::FilesOnly)) {
        // this->_wares_json.push_back(Ware::load(file_iterator.absoluteFilePath().toStdString()));

        spdlog::info("loaded : {}", file_iterator.fileName().toStdString());
    }
}

void Loader::_load_modules() {
    spdlog::info("Loading modules");

    for (const auto &file_iterator: QDirListing(QString("assets/modules"), QDirListing::IteratorFlag::FilesOnly)) {
        try {
            spdlog::info("loading: {}", file_iterator.fileName().toStdString());
            std::fstream file(file_iterator.absoluteFilePath().toStdString(), std::fstream::in);
            if (!file.is_open())
                throw std::runtime_error("Failed to open file " + file_iterator.fileName().toStdString());
            if (file.bad()) {
                throw std::runtime_error("Failed to read file " + file_iterator.fileName().toStdString());
            }


            auto j = nlohmann::json::parse(file);
            this->_modules_json.push_back(j.get<TmpModule>());
            file.close();

        } catch (const std::exception &e) {
            spdlog::error("could not parse {} : {}", file_iterator.absoluteFilePath().toStdString(), e.what());
        }
    }
}

void Loader::_parse_wares() {
}

void Loader::load() {
    this->_load_wares();
    this->_load_modules();
}
