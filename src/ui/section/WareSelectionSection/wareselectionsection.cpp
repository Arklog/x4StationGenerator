//
// Created by pierre on 7/16/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_wareselectionsection.h" resolved

#include "wareselectionsection.h"

#include "ui_wareselectionsection.h"
#include "Data/Data.hpp"

#include"spdlog/spdlog.h"

#include "ui/section/WareSelectionSection/widgets/wareconfiguratorpanel.hpp"
#include "ui/section/WareSelectionSection/widgets/waresselector.hpp"


WareSelectionSection::WareSelectionSection(Settings &settings, QWidget *parent) : QWidget(parent),
    ui(new Ui::WareSelectionSection), settings_{settings} {
    ui->setupUi(this);
    auto ware_selector           = new WaresSelector(this);
    auto ware_configurator_panel = new WareConfiguratorPanel(settings, this);

    for (auto const &[module_id, module]: Data::modules->habitation_map) {
        ui->habitat_input->addItem(QString::fromStdString(module->name));
    }

    ui->main_layout->setColumnStretch(0, 2);
    ui->main_layout->setColumnStretch(1, 1);
    ui->main_layout->addWidget(ware_selector, 0, 0);
    ui->main_layout->addWidget(ware_configurator_panel, 0, 1);

    connect(ware_selector, &WaresSelector::wareSelected, [this, ware_configurator_panel](t_ware_id ware_id) {
        ware_configurator_panel->addWare(ware_id, false, 0);
    });
    connect(ware_configurator_panel, &WareConfiguratorPanel::shouldUpdate, [this](t_x4_complex complex) {
        this->complex_ = std::move(complex);
        emit complexUpdated();
    });
    connect(ui->workforce_input, &QCheckBox::toggled, [this, ware_configurator_panel](bool checked) {
        this->settings_.workforce_enables = checked;
        spdlog::debug("workforce enabled: {}", this->settings_.workforce_enables);
        ware_configurator_panel->productionTargetUpdate();
    });
    connect(ui->habitat_input, &QComboBox::currentTextChanged, [this, ware_configurator_panel](QString text) {
        const auto &modules = Data::modules->module_name_map;
        auto        name    = text.toStdString();

        this->settings_.workforce_module = modules.at(name)->id;
        spdlog::debug("default habitat changed: {}", this->settings_.workforce_module);
        ware_configurator_panel->productionTargetUpdate();
    });
    ui->habitat_input->currentTextChanged(ui->habitat_input->currentText());
}

WareSelectionSection::~WareSelectionSection() {
    delete ui;
}

const t_x4_complex &WareSelectionSection::getComplex() {
    return this->complex_;
}
