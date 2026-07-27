//
// Created by pierre on 7/21/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DockAndPierrConfigurationPanel.h" resolved

#include "moduleconfiguration.hpp"
#include "moduleconfigurationpanel.hpp"
#include "ui_moduleconfigurationpanel.h"
#include "utils/SharedState.hpp"


ModuleConfigurationPanel::ModuleConfigurationPanel(ui::utils::SharedState &state, module_list_target member_target,
                                                   QWidget *               parent) :
QFrame(parent),
ui(new Ui::ModuleConfigurationPanel),
state_(state),
member_(member_target) {
    QFrame::setFrameShape(QFrame::StyledPanel);
    ui->setupUi(this);
    ui->layout->setAlignment(Qt::AlignTop);
}

ModuleConfigurationPanel::~ModuleConfigurationPanel() {
    delete ui;
}

void ModuleConfigurationPanel::addModule(const Module *module) {
    auto  managed_settings = state_.settings();
    auto  settings         = &managed_settings.get();
    auto &module_targets_  = settings->*member_;

    auto iter = std::find(module_targets_.begin(), module_targets_.end(), module->id);
    if (iter != module_targets_.end())
        return;

    auto &module_target = module_targets_.emplace_back(module->id, 1);
    auto  widget        = new ModuleConfiguration(module, module_target, this);

    ui->layout->addWidget(widget);

    connect(widget, &ModuleConfiguration::shouldRemove, [this, widget, module_target]() -> void {
        ui->layout->removeWidget(widget);
        auto  managed_settings = this->state_.settings();
        auto  settings         = &managed_settings.get();
        auto &module_targets_  = settings->*(this->member_);

        const auto iter = std::find(module_targets_.begin(), module_targets_.end(), module_target);
        module_targets_.erase(iter);

        delete widget;
    });
    connect(widget, &ModuleConfiguration::moduleTargetUpdated, [this]() {
        emit state_.settingsChanged(state_.settings());
    });
}
