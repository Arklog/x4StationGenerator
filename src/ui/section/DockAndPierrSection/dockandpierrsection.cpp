//
// Created by pierre on 7/21/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DockAndPierrSection.h" resolved

#include "dockandpierrsection.hpp"

#include <QLayout>

#include "ui_dockandpierrsection.h"

#include "../../widgets/moduleconfigurationpanel.hpp"
#include "../../widgets/moduleselectionpanel.hpp"


DockAndPierrSection::DockAndPierrSection(QWidget *parent) : QWidget(parent), ui(new Ui::DockAndPierrSection) {
    ui->setupUi(this);

    ui->gridLayout_2->setColumnStretch(0, 1);
    ui->gridLayout_2->setColumnStretch(1, 2);

    auto dock_and_pierr_selection_panel = new ModuleSelectionPanel(this);
    auto dock_and_pierr_configuration_panel = new ModuleConfigurationPanel(this);

    ui->gridLayout_2->addWidget(dock_and_pierr_selection_panel, 0, 0, 1, 1);
    ui->gridLayout_2->addWidget(dock_and_pierr_configuration_panel, 0, 1, 1, 1);

    this->dock_and_pierr_configuration_panel = dock_and_pierr_configuration_panel;
    connect(dock_and_pierr_selection_panel, &ModuleSelectionPanel::moduleSelected, dock_and_pierr_configuration_panel, &ModuleConfigurationPanel::addDockOrPierr);
}

DockAndPierrSection::~DockAndPierrSection() {
    delete ui;
}

t_module_target_list DockAndPierrSection::getModuleTargetList() const {
    return dock_and_pierr_configuration_panel->getDocksAndPierr();
}
