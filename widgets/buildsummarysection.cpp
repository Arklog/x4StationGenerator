//
// Created by pierre on 2/15/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_BuildSummarySection.h" resolved

#include "buildsummarysection.hpp"
#include "widgets/ui_buildsummarysection.h"


BuildSummarySection::BuildSummarySection(QWidget *parent) :
        QWidget(parent), ui(new Ui::BuildSummarySection)
{
    ui->setupUi(this);
    connect(ui->export_button, &QPushButton::clicked, this, &BuildSummarySection::exportStation);
}

BuildSummarySection::~BuildSummarySection()
{
    delete ui;
}

void BuildSummarySection::modulesUpdated(const StationBuilder &builder)
{
    auto modules_map = builder.getModulesMap();

    clearLayout(ui->modules);

    for (auto const &iter: modules_map) {
        auto name       = iter.first.name + " :";
        auto name_label = new QLabel(name.c_str());
        auto qty_label  = new QLabel(std::to_string(iter.second).c_str());

        name_label->setStyleSheet("font-weight: bold;");
        ui->modules->addRow(name_label, qty_label);
    }
}
