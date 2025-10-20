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
                                             QWidget *       parent)
    : QGroupBox(parent), ui(new Ui::WareConfiguratorPanel),
      ware_configurators{}, ware_target_container{}, settings_(settings) {
    ui->setupUi(this);
    // QGroupBox::setFrameShape(QFrame::StyledPanel);
    this->setWindowTitle({"Configuration"});

    auto layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    this->setLayout(layout);
}

WareConfiguratorPanel::~WareConfiguratorPanel() { delete ui; }

void WareConfiguratorPanel::addWare(t_ware_id ware_id, bool is_secondary, unsigned amount) {
    spdlog::debug("[WareConfiguratorPanel] adding ware {} as {} target with amount {}",
                  ware_id.raw(), is_secondary ? "secondary" : "primary", amount);

    // Skip adding secondary target if already primary
    if (this->ware_target_container.isPrimaryTarget(ware_id)) {
        spdlog::info("{} is already a primary target, skipping", ware_id.raw());
        return;
    }

    // If already secondary, remove secondary widget and replace by primary
    if (this->ware_target_container.isSecondaryTarget(ware_id) && !is_secondary) {
        spdlog::info("[WareConfiguratorPanel] {} is already a secondary target, upgrading to primary",
                     ware_id.raw());
        auto widget = this->ware_configurators[ware_id];
        this->layout()->removeWidget(widget);
        delete widget;
    }

    // Create a new ware configurator
    WareConfigurator *ware_configurator = nullptr;
    if (!is_secondary) {
        this->ware_target_container.setPrimaryTarget(ware_id);
        auto ware_target  = this->ware_target_container.getPrimaryTarget(ware_id);
        ware_configurator = new WareConfigurator(ware_target, this);
    } else {
        this->ware_target_container.setSecondaryTarget(ware_id);
        auto ware_target       = this->ware_target_container.getSecondaryTarget(ware_id);
        ware_target->prodution = amount;
        ware_configurator      = new WareConfigurator(ware_target, this);
    }

    // Store the configurator and add it to the layout
    this->ware_configurators[ware_id] = ware_configurator;
    this->layout()->addWidget(ware_configurator);

    connect(
        ware_configurator, &WareConfigurator::shouldRemove,
        [this](t_ware_id ware_id) -> void {
            if (!this->ware_target_container.isPrimaryTarget(ware_id)) {
                spdlog::error("Ware {} is not a primary target, cannot remove", ware_id.raw());
                throw std::logic_error("Ware is not a primary target, cannot remove");
            }

            auto widget = this->ware_configurators[ware_id];

            this->layout()->removeWidget(widget);
            this->ware_target_container.unsetPrimaryTarget(ware_id);
            this->ware_configurators.erase(ware_id);
            delete widget;

            this->productionTargetUpdate();
        });
    connect(ware_configurator, &WareConfigurator::shouldUpdate, this,
            &WareConfiguratorPanel::productionTargetUpdate);

    // Cleanup ui
    if (!is_secondary) {
        this->productionTargetUpdate();
    }
}

void WareConfiguratorPanel::productionTargetUpdate() {
    spdlog::debug("[{}]: updating production targets", __PRETTY_FUNCTION__);

    ComplexGeneratorBase test(settings_, this->ware_target_container);
    auto                 build_result = test.build();

    const auto &current_production = test.getCurrentProduction();

    spdlog::debug("[{}]: removing all secondary targets", __PRETTY_FUNCTION__);
    // Delete all secondary targets here
    std::vector<t_ware_id> to_remove{};
    for (const auto &[ware_id, widget]: this->ware_configurators) {
        spdlog::debug("[{}]: checking ware {} for removal", __PRETTY_FUNCTION__, ware_id);

        if (!widget->getWareTarget()->is_secondary) {
            spdlog::debug("[{}]: ware {} is primary, skipping", __PRETTY_FUNCTION__, ware_id);
            continue;
        }

        spdlog::debug("[{}]: removing ware {} from ui", __PRETTY_FUNCTION__, ware_id);
        this->layout()->removeWidget(widget);
        // delete widget;

        to_remove.push_back(ware_id);
    }

    spdlog::debug("[{}]: cleaning up secondary targets from container", __PRETTY_FUNCTION__);
    for (const auto &ware_id: to_remove) {
        this->ware_configurators.erase(ware_id);
    }

    spdlog::debug("[{}]: adding secondary targets to ui", __PRETTY_FUNCTION__);
    // Add secondary targets
    for (const auto &ware_target: current_production.getSecondaryTargets()) {
        this->addWare(ware_target->ware_id, true, ware_target->prodution);
    }

    emit shouldUpdate(build_result);
}
