//
// Created by pierre on 7/17/25.
//

// You may need to build the project (run Qt uic code generator) to get
// "ui_WareConfiguratorPanel.h" resolved

#include "wareconfiguratorpanel.hpp"

#include <QVBoxLayout>

#include "ui_wareconfiguratorpanel.h"
#include "wareconfigurator.hpp"

#include "StationBuilder/Generator/ComplexGeneratorBase.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bundled/chrono.h"

WareConfiguratorPanel::WareConfiguratorPanel(const Settings &settings,
                                             QWidget *parent)
    : QGroupBox(parent), ui(new Ui::WareConfiguratorPanel),
      ware_configurators{}, ware_target_container{}, ware_targets{},
      settings_(settings) {
    ui->setupUi(this);
    // QGroupBox::setFrameShape(QFrame::StyledPanel);
    this->setWindowTitle({"Configuration"});

    auto layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    this->setLayout(layout);
}

WareConfiguratorPanel::~WareConfiguratorPanel() { delete ui; }

void WareConfiguratorPanel::addWare(t_ware_id ware_id, bool is_secondary, unsigned amount) {
    if (this->ware_target_container.isPrimaryTarget(ware_id)) {
        spdlog::info("{} is already a primary target, skipping", ware_id.raw());
        return;
    }

    // Create a new ware configurator
    WareConfigurator *ware_configurator = nullptr;
    if (!is_secondary) {
        this->ware_target_container.setPrimaryTarget(ware_id);
        auto ware_target = this->ware_target_container.getPrimaryTarget(ware_id);
        ware_configurator = new WareConfigurator(ware_target, this);
    } else {
        this->ware_target_container.setSecondaryTarget(ware_id);
        auto ware_target = this->ware_target_container.getSecondaryTarget(ware_id);
        ware_target->prodution = amount;
        ware_configurator = new WareConfigurator(ware_target, this);
    }

    // Store the configurator and add it to the layout
    this->ware_configurators[ware_id] = ware_configurator;
    this->ware_targets.push_back(ware_configurator->getWareTarget());
    this->layout()->addWidget(ware_configurator);

    connect(
        ware_configurator, &WareConfigurator::shouldRemove,
        [this](t_ware_id wid) -> void {
            if (!this->ware_target_container.isPrimaryTarget(wid)) {
                spdlog::error("Ware {} is not a primary target, cannot remove", wid.raw());
                throw std::logic_error("Ware is not a primary target, cannot remove");
            }

            auto widget = this->ware_configurators[wid];

            this->ware_configurators.erase(wid);
            for (auto iter = this->ware_targets.begin();
                 iter != this->ware_targets.end(); ++iter) {
                if (*iter == widget->getWareTarget()) {
                    this->ware_targets.erase(iter);
                    break;
                }
            }
            this->layout()->removeWidget(widget);
            this->ware_target_container.unsetPrimaryTarget(wid);
            delete widget;

            this->productionTargetUpdate();
        });
    connect(ware_configurator, &WareConfigurator::shouldUpdate, this,
            &WareConfiguratorPanel::productionTargetUpdate);
}

void WareConfiguratorPanel::productionTargetUpdate() {
    ComplexGeneratorBase test(settings_, this->ware_target_container);
    auto build_result = test.build();

    const auto &current_production = test.getCurrentProduction();

    // Delete all secondary targets here
    for (const auto &[ware_id, widget]: this->ware_configurators) {
        if (!widget->getWareTarget()->is_secondary)
            continue;

        this->layout()->removeWidget(widget);
        delete widget;
    }

    // Read secondary targets
    for (const auto &ware_target: current_production.getSecondaryTargets()) {
        this->addWare(ware_target->ware_id, true, ware_target->prodution);
    }

    emit shouldUpdate(build_result);
}
