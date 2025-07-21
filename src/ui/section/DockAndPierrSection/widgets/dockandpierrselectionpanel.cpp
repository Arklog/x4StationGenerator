//
// Created by pierre on 7/21/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DockAndPierrSelectionPanel.h" resolved

#include "dockandpierrselectionpanel.hpp"

#include <QPushButton>

#include "ui_dockandpierrselectionpanel.h"

#include "Data/WaresAndModules.hpp"


DockAndPierrSelectionPanel::DockAndPierrSelectionPanel(QWidget *parent) :
    QFrame(parent), ui(new Ui::DockAndPierrSelectionPanel) {
    QFrame::setFrameShape(QFrame::StyledPanel);

    ui->setupUi(this);

    const auto& module = getModules();

    for (const auto& iterator : module) {
        const auto& key = iterator.first;
        const auto& module = iterator.second;

        if (module->type != "pierr" && module->type != "dockarea")
            continue;

        auto module_button = new QPushButton(QString::fromStdString(module->name));
        this->layout()->addWidget(module_button);

        connect(module_button, &QPushButton::clicked, [this, &module](bool clicked) -> void {
            emit moduleSelected(module);
        });
    }
}

DockAndPierrSelectionPanel::~DockAndPierrSelectionPanel() {
    delete ui;
}
