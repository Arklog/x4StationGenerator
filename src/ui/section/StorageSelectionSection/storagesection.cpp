//
// Created by pierre on 7/22/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_StorageSelectionSection.h" resolved

#include "storagesection.hpp"

#include "ui_storagesection.h"

#include "Data/WareModuleAndWorkforce.hpp"

#include "ui/widgets/moduleconfigurationpanel.hpp"
#include "ui/widgets/moduleselectionpanel.hpp"

#include <QScrollArea>

StorageSection::StorageSection(QWidget *parent) :
    QWidget(parent), ui(new Ui::StorageSection) {
    ui->setupUi(this);

    const auto& modules = getModules();
    t_module_list storage_list{};

    for (const auto& iter: modules) {
        const auto& key = iter.first;
        const auto& module = iter.second;

        if (module->type == ModuleType::storage)
            storage_list.insert(module);
    }

    auto storage_selection_panel = new ModuleSelectionPanel(storage_list, this);
    auto storage_configuration_panel = new ModuleConfigurationPanel(this);
    this->storage_selection_panel = storage_selection_panel;
    this->storage_configuration_panel = storage_configuration_panel;

    ui->selection_scroll_area->setWidget(storage_selection_panel);
    ui->selection_scroll_area->setWidgetResizable(true);
    ui->selection_scroll_area->setLayoutDirection(Qt::RightToLeft);
    ui->configuration_scroll_area->setWidget(storage_configuration_panel);
    ui->configuration_scroll_area->setWidgetResizable(true);
    ui->configuration_scroll_area->setLayoutDirection(Qt::RightToLeft);

    connect(storage_selection_panel, &ModuleSelectionPanel::moduleSelected, storage_configuration_panel, &ModuleConfigurationPanel::addModule);
    connect(storage_configuration_panel, &ModuleConfigurationPanel::targetListUpdated, this, &StorageSection::storageUpdated);
}

StorageSection::~StorageSection() {
    delete ui;
}

t_module_target_list StorageSection::getModuleTargetList() const {
    return this->storage_configuration_panel->getModuleTargets();
}
