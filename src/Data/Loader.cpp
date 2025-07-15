//
// Created by pierre on 7/15/25.
//

#include "Loader.h"

#include <fstream>
#include <iostream>
#include <qdirlisting.h>
#include <qstring.h>

#include "nlohmann/json.hpp"

void Loader::_load_wares() {

    for (const auto &file_iterator: QDirListing(QString("assets/wares/"), QDirListing::IteratorFlag::FilesOnly)) {
        std::cout << file_iterator.absoluteFilePath().toStdString() << std::endl;

        auto file = std::ifstream(file_iterator.absoluteFilePath().toStdString());
        this->_wares_json.push_back(nlohmann::json::parse(file));
    }
}

void Loader::_load_modules() {
    for (const auto &file_iterator: QDirListing(QString("assets/modules"), QDirListing::IteratorFlag::FilesOnly)) {
        std::cout << file_iterator.absoluteFilePath().toStdString() << std::endl;

        auto file = std::ifstream(file_iterator.absoluteFilePath().toStdString());
        this->_modules_json.push_back(nlohmann::json::parse(file));
    }
}

void Loader::load() {
    this->_load_wares();
    this->_load_modules();
}
