//
// Created by pierre on 7/21/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DockAndPierrSelectionPanel.h" resolved

#include "dockandpierrselectionpanel.hpp"

#include <QPushButton>

#include "ui_dockandpierrselectionpanel.h"

#include "Data/WareModuleAndWorkforce.hpp"


DockAndPierrSelectionPanel::DockAndPierrSelectionPanel(QWidget *parent) :
    QFrame(parent), ui(new Ui::DockAndPierrSelectionPanel) {
    QFrame::setFrameShape(QFrame::StyledPanel);

    ui->setupUi(this);

    const auto& module = getModules();
    std::vector<const TmpModule*> pierr_and_dock{};

    for (const auto& iterator : module) {
        const auto& key = iterator.first;
        const auto& module = iterator.second;

        if (module->type == "pierr" || module->type == "dockarea") {
            pierr_and_dock.push_back(module);
        }
    }

    std::sort(pierr_and_dock.begin(), pierr_and_dock.end(), [] (const TmpModule *a, const TmpModule *b) {
        return a->name < b->name;
    });

    for (const auto& module: pierr_and_dock) {
        auto module_button = new QPushButton(QString::fromStdString(module->name));
        this->layout()->addWidget(module_button);

        connect(module_button, &QPushButton::clicked, [this, module](bool clicked) -> void {
            emit moduleSelected(module);
        });
    }
}

DockAndPierrSelectionPanel::~DockAndPierrSelectionPanel() {
    delete ui;
}
