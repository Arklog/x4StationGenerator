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
        auto file = std::ifstream(file_iterator.absoluteFilePath().toStdString());
        this->_wares_json.push_back(Ware::load(file_iterator.absoluteFilePath().toStdString()));

        spdlog::info("loaded : {}", file_iterator.fileName().toStdString());
    }
}

void Loader::_load_modules() {
    spdlog::info("Loading modules");

    for (const auto &file_iterator: QDirListing(QString("assets/modules"), QDirListing::IteratorFlag::FilesOnly)) {
        auto file = std::ifstream(file_iterator.absoluteFilePath().toStdString());
        this->_modules_json.push_back(nlohmann::json::parse(file));

        spdlog::info("loaded : {}", file_iterator.fileName().toStdString());
    }
}

void Loader::_parse_wares() {
}

void Loader::load() {
    this->_load_wares();
    this->_load_modules();
}
