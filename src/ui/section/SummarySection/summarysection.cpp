//
// Created by pierre on 7/25/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SummarySection.h" resolved

#include "summarysection.hpp"
#include "ui_summarysection.h"

#include "Data/WareModuleAndWorkforce.hpp"
#include <QString>
#include <QLabel>

#include "spdlog/spdlog.h"

SummarySection::SummarySection(QWidget *parent) :
    QWidget(parent), ui(new Ui::SummarySection) {
    ui->setupUi(this);
}

SummarySection::~SummarySection() {
    delete ui;
}

void SummarySection::updateTargetList(const t_x4_complex &targets) {
    auto modules = getModules();

    spdlog::debug("Generating summary");
    for (const auto &target : targets) {
        spdlog::debug("{}", target);
        const auto& module = modules.at(target);
        auto label = new QLabel(QString::fromStdString(module->name), this);
        ui->layout->addWidget(label);
    }
}
