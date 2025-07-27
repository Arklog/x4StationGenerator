//
// Created by pierre on 7/21/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DockAndPierrConfigurationPanel.h" resolved

#include "moduleconfiguration.hpp"
#include "moduleconfigurationpanel.hpp"
#include "ui_moduleconfigurationpanel.h"


ModuleConfigurationPanel::ModuleConfigurationPanel(QWidget *parent) : QFrame(parent),
                                                                      ui(new Ui::ModuleConfigurationPanel) {
    QFrame::setFrameShape(QFrame::StyledPanel);
    ui->setupUi(this);
    ui->layout->setAlignment(Qt::AlignTop);
}

ModuleConfigurationPanel::~ModuleConfigurationPanel() {
    delete ui;
}

t_module_target_list ModuleConfigurationPanel::getModuleTargets() const {
    t_module_target_list docks_and_pierr_list{};

    for (auto i = 0; i < ui->layout->count(); ++i) {
        auto item = ui->layout->itemAt(i);
        auto widget = item->widget();

        if (!widget)
            continue;

        auto config = static_cast<ModuleConfiguration *>(widget);
        auto target = config->getModuleTarget();

        if (target.amount == 0)
            continue;
        docks_and_pierr_list.push_back(config->getModuleTarget());
    }

    return docks_and_pierr_list;
}

void ModuleConfigurationPanel::addModule(const Module *dock_or_pierr) {
    auto iter = std::find(module_targets_.begin(), module_targets_.end(), dock_or_pierr->id);
    if (iter != module_targets_.end())
        return;

    auto &module_target = this->module_targets_.emplace_back(dock_or_pierr->id, 1);
    auto widget = new ModuleConfiguration(dock_or_pierr, module_target, this);

    ui->layout->addWidget(widget);

    connect(widget, &ModuleConfiguration::shouldRemove, [this, widget, module_target]() -> void {
        ui->layout->removeWidget(widget);
        const auto iter = std::find(module_targets_.begin(), module_targets_.end(), module_target);
        this->module_targets_.erase(iter);

        delete widget;
    });

    connect(widget, &ModuleConfiguration::moduleTargetUpdated, [this] (const ModuleTarget& target) {
        emit this->targetListUpdated();
    });

    emit targetListUpdated();
}
