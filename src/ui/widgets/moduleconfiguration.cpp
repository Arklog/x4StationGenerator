//
// Created by pierre on 7/21/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DockAndPierrConfiguration.h" resolved

#include "moduleconfiguration.hpp"
#include "ui_moduleconfiguration.h"

#include "Data/Data.hpp"


ModuleConfiguration::ModuleConfiguration(const Module *dock_or_pierr, QWidget *parent) :
    QWidget(parent), ui(new Ui::ModuleConfiguration), dock_or_pierr{dock_or_pierr} {
    ui->setupUi(this);

    ui->name_label->setText(QString::fromStdString(dock_or_pierr->name));

    connect(ui->remove_button, &QPushButton::clicked, [this] (bool clicked) -> void {
        emit shouldRemove();
    });
}

ModuleConfiguration::~ModuleConfiguration() {
    delete ui;
}

ModuleTarget ModuleConfiguration::getModuleTarget() const {
    return {.module_id = dock_or_pierr->id, .amount = static_cast<size_t>(ui->quantity->value())};
}
