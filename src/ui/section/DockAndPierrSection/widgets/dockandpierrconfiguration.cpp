//
// Created by pierre on 7/21/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DockAndPierrConfiguration.h" resolved

#include "dockandpierrconfiguration.hpp"
#include "ui_dockandpierrconfiguration.h"

#include "Data/Data.hpp"


DockAndPierrConfiguration::DockAndPierrConfiguration(const TmpModule *dock_or_pierr, QWidget *parent) :
    QWidget(parent), ui(new Ui::DockAndPierrConfiguration), dock_or_pierr{dock_or_pierr} {
    ui->setupUi(this);

    ui->name_label->setText(QString::fromStdString(dock_or_pierr->name));

    connect(ui->remove_button, &QPushButton::clicked, [this] (bool clicked) -> void {
        emit shouldRemove();
    });
}

DockAndPierrConfiguration::~DockAndPierrConfiguration() {
    delete ui;
}

ModuleTarget DockAndPierrConfiguration::getModuleTarget() const {
    return {.module_id = dock_or_pierr->id, .amount = ui->quantity->value()};
}
