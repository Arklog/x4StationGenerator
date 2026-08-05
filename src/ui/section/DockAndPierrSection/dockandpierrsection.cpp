//
// Created by pierre on 7/21/25.
//

// You may need to build the project (run Qt uic code generator) to get
// "ui_DockAndPierrSection.h" resolved

#include "dockandpierrsection.hpp"

#include <QLayout>

#include "ui_dockandpierrsection.h"

#include "../../widgets/moduleconfigurationpanel.hpp"
#include "../../widgets/moduleselectionpanel.hpp"

#include "data/WareModuleAndWorkforce.hpp"
#include "utils/SharedState.hpp"

DockAndPierrSection::DockAndPierrSection(ui::utils::SharedState &state, const Store &store, QWidget *parent) :
QWidget(parent),
ui(new Ui::DockAndPierrSection),
state_(state),
store_(store) {
    ui->setupUi(this);

    common::data::t_module_list dock_and_pierr_list{};

    for (const auto &dock: store.docks.datas) {
        dock_and_pierr_list.insert(&dock.module.get());
    }
    for (const auto &pier: store.piers.datas) {
        dock_and_pierr_list.insert(&pier.module.get());
    }

    auto dock_and_pierr_selection_panel     = new ModuleSelectionPanel(dock_and_pierr_list, this);
    auto dock_and_pierr_configuration_panel = new ModuleConfigurationPanel(
        state_, &common::stationbuilder::Settings::docks, this);

    ui->dock_and_pierr_selection_scroll_area->setWidget(dock_and_pierr_selection_panel);
    ui->dock_and_pierr_selection_scroll_area->setWidgetResizable(true);
    ui->dock_and_pierr_selection_scroll_area->setLayoutDirection(Qt::RightToLeft);

    ui->dock_and_pierr_configuration_scroll_area->setWidget(dock_and_pierr_configuration_panel);
    ui->dock_and_pierr_configuration_scroll_area->setWidgetResizable(true);
    ui->dock_and_pierr_configuration_scroll_area->setLayoutDirection(Qt::RightToLeft);

    this->dock_and_pierr_configuration_panel
            = dock_and_pierr_configuration_panel;
    connect(dock_and_pierr_selection_panel,
            &ModuleSelectionPanel::moduleSelected,
            dock_and_pierr_configuration_panel,
            &ModuleConfigurationPanel::addModule);
    connect(&this->state_, &ui::utils::SharedState::saveFileLoaded, [this]() {
        this->dock_and_pierr_configuration_panel->loadPlan(this->store_);
    });
}

DockAndPierrSection::~DockAndPierrSection() { delete ui; }
