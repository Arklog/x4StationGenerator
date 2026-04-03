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

#include "Data/WareModuleAndWorkforce.hpp"

DockAndPierrSection::DockAndPierrSection (const Store &store, QWidget *parent)
    : QWidget (parent), ui (new Ui::DockAndPierrSection), store_ (store)
{
    ui->setupUi (this);

    t_module_list dock_and_pierr_list{};

    for (const auto &module : store.modules.docks_and_piers)
    {
	dock_and_pierr_list.insert (module);
    }

    auto dock_and_pierr_selection_panel
	= new ModuleSelectionPanel (dock_and_pierr_list, this);
    auto dock_and_pierr_configuration_panel
	= new ModuleConfigurationPanel (this);

    ui->dock_and_pierr_selection_scroll_area->setWidget (
	dock_and_pierr_selection_panel);
    ui->dock_and_pierr_selection_scroll_area->setWidgetResizable (true);
    ui->dock_and_pierr_selection_scroll_area->setLayoutDirection (
	Qt::RightToLeft);

    ui->dock_and_pierr_configuration_scroll_area->setWidget (
	dock_and_pierr_configuration_panel);
    ui->dock_and_pierr_configuration_scroll_area->setWidgetResizable (true);
    ui->dock_and_pierr_configuration_scroll_area->setLayoutDirection (
	Qt::RightToLeft);

    this->dock_and_pierr_configuration_panel
	= dock_and_pierr_configuration_panel;
    connect (dock_and_pierr_selection_panel,
	     &ModuleSelectionPanel::moduleSelected,
	     dock_and_pierr_configuration_panel,
	     &ModuleConfigurationPanel::addModule);
    connect (dock_and_pierr_configuration_panel,
	     &ModuleConfigurationPanel::targetListUpdated, this,
	     &DockAndPierrSection::dockAndPierrUpdated);
}

DockAndPierrSection::~DockAndPierrSection () { delete ui; }

t_module_target_list DockAndPierrSection::getModuleTargetList () const
{
    return dock_and_pierr_configuration_panel->getModuleTargets ();
}
