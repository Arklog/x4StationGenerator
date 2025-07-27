//
// Created by pierre on 2/1/25.
//
#include "modules.hpp"

#include <stack>
#include <chrono>
#include <QUuid>
#include "Data/WareModuleAndWorkforce.hpp"

#include "ui/section/DockAndPierrSection/dockandpierrsection.hpp"

std::string genModulePlan(const t_x4_complex &complex, Settings &settings) {
    std::stringstream plan;
    StationSize station_size{.x_plus = 10, .x_minus = 10, .y_plus = 10, .y_minus = 10, .z_plus = 10, .z_minus = 10};
    const auto &modules = getModules();

    int xpos_pierr = 2500 - 1000 * station_size.x_minus;
    int xpos_dock = 2700 - 1000 * station_size.x_minus;
    std::stack<std::array<int, 3> > positions_dock{};
    std::stack<std::array<int, 3> > positions_pier{};

    // meters to kilometers
    station_size.x_plus *= 1000;
    station_size.x_minus *= 1000;
    station_size.y_plus *= 1000;
    station_size.y_minus *= 1000;
    station_size.z_plus *= 1000;
    station_size.z_minus *= 1000;

    for (int i = 200 - station_size.y_minus; i < station_size.y_plus; i += 2350) {
        for (int j = 200 - station_size.z_minus; j < station_size.z_plus; j += 2350) {
            positions_dock.emplace(std::array<int, 3>{xpos_dock, i, j});
            positions_pier.emplace(std::array<int, 3>{xpos_pierr, i, j});
        }
    }

    // std::random_device rd;
    // std::mt19937 eng{rd()};
    // UUIDv4::UUIDGenerator<std::mt19937_64> gen{};
    // auto uuid = gen.getUUID();
    auto uuid = QUuid::createUuid().toString().toStdString();
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

    for (auto &c: uuid)
        c = toupper(c);
    uuid.erase(uuid.begin());
    uuid.erase(uuid.end() - 1);

    plan << R"(<?xml version="1.0" encoding="UTF-8"?>)" << std::endl;
    plan << R"(<plans>)" << std::endl;
    plan << R"(  <plan id=")" << uuid << "_" << timestamp << R"(" name=")" << settings.name
            << R"(" description="">)" << std::endl;

    unsigned int i = 1;
    for (auto const &module_id: complex) {
        auto const &module = modules.at(module_id);

        std::array<int, 3> pos{0, 0, 0};
        if (module->type == ModuleType::pier) {
            pos = positions_pier.top();
            positions_pier.pop();
        } else if (module->type == ModuleType::dock) {
            pos = positions_dock.top();
            positions_dock.pop();
        }

        plan << "    <entry " << "index=\"" << i << "\" macro=\"" << module->macro << "\">" << std::endl;
        plan << "      <offset>" << std::endl;
        plan << R"(        <position x=")" << pos[0] << R"(" y=")" << pos[1] << R"(" z=")" << pos[2] << R"("/>)"
                << std::endl;
        plan << "      </offset>" << std::endl;
        plan << "    </entry>" << std::endl;
    }

    plan << R"(  </plan>)" << std::endl;
    plan << R"(</plans>)" << std::endl;
    return plan.str();
}
