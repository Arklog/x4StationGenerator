//
// Created by pierre on 7/15/25.
//

#include "Loader.hpp"

#include <fstream>
#include <iostream>
#include <qdirlisting.h>
#include <qstring.h>

#include "spdlog/spdlog.h"
#include "nlohmann/json.hpp"

static const auto module_path = QString("assets/modules");
static const auto workforce_path = QString("assets/workforce.json");
// static const auto module_path = QString("tools/regenerate_ware_and_modules/output/modules");

void Loader::_loadModules() {
    spdlog::info("Loading modules");

    for (const auto &file_iterator: QDirListing(module_path, QDirListing::IteratorFlag::FilesOnly)) {
        try {
            spdlog::info("loading: {}", file_iterator.fileName().toStdString());
            std::fstream file(file_iterator.absoluteFilePath().toStdString(), std::fstream::in);
            if (!file.is_open())
                throw std::runtime_error("Failed to open file " + file_iterator.fileName().toStdString());
            if (file.bad()) {
                throw std::runtime_error("Failed to read file " + file_iterator.fileName().toStdString());
            }


            auto j = nlohmann::json::parse(file);
            this->_modules_json.push_back(j.get<Module>());
            file.close();

        } catch (const std::exception &e) {
            spdlog::error("could not parse {} : {}", file_iterator.absoluteFilePath().toStdString(), e.what());
        }
    }
}

void Loader::_loadWorkforce() {
    spdlog::info("Loading workforce");

    try {
        std::ifstream file(workforce_path.toStdString(), std::ifstream::in);
        if (!file.is_open())
            throw std::runtime_error("Failed to open file " + workforce_path.toStdString());
        if (file.bad())
            throw std::runtime_error("Failed to read file " + workforce_path.toStdString());

        auto j = nlohmann::json::parse(file);
        this->_workforce = j.get<std::map<std::string, std::vector<std::pair<t_ware_id, double>>>>();
        spdlog::debug("found {} workforce", this->_workforce.size());
    } catch (const std::exception &e) {
        spdlog::error("could not parse workforce");
        throw;
    }
}

void Loader::_parse_wares() {
}

void Loader::load() {
    this->_loadModules();
    this->_loadWorkforce();
}
