//
// Created by pierre on 7/16/25.
//

// You may need to build the project (run Qt uic code generator) to get
// "ui_wareselectionsection.h" resolved

#include "wareselectionsection.h"

#include "ui_wareselectionsection.h"

#include "spdlog/spdlog.h"

#include "data/WareModuleAndWorkforce.hpp"

#include "section/WareSelectionSection/widgets/wareconfiguratorpanel.hpp"
#include "section/WareSelectionSection/widgets/waresselector.hpp"

WareSelectionSection::WareSelectionSection(Settings &   settings,
                                           const Store &store, QWidget *parent) :
QWidget(parent),
ui(new Ui::WareSelectionSection),
settings_{settings},
store_(store) {
    ui->setupUi(this);
    auto ware_selector           = new WaresSelector(store, this);
    auto ware_configurator_panel = new WareConfiguratorPanel(settings, store, this);

    for (auto const &habitat: store.habitats.datas) {
        ui->habitat_input->addItem(QString::fromStdString(habitat.module.value().name));
    }

    ui->main_layout->setColumnStretch(0, 2);
    ui->main_layout->setColumnStretch(1, 1);
    ui->main_layout->addWidget(ware_selector, 0, 0);
    ui->main_layout->addWidget(ware_configurator_panel, 0, 1);
    ui->habitat_input->currentTextChanged(ui->habitat_input->currentText());

    connect(ware_selector, &WaresSelector::wareSelected,
            [this, ware_configurator_panel](t_ware_id ware_id) {
                ware_configurator_panel->addWare(ware_id, false, 0);
            });
    connect(ware_configurator_panel, &WareConfiguratorPanel::shouldUpdate,
            [this](common::stationbuilder::Complex complex) {
                this->complex_ = std::move(complex);
                emit complexUpdated();
            });
    connect(ui->workforce_input, &QCheckBox::toggled,
            [this, ware_configurator_panel](bool checked) {
                this->settings_.workforce_enables = checked;
                spdlog::debug("workforce enabled: {}",
                              this->settings_.workforce_enables);
                ware_configurator_panel->productionTargetUpdate();
            });
    connect(ui->habitat_input, &QComboBox::currentTextChanged,
            [this, ware_configurator_panel](QString text) {
                auto module                      = this->store_.modules.by_name.at(text.toStdString());
                this->settings_.workforce_module = module->module.get().id;
                spdlog::debug("default habitat changed: {}",
                              this->settings_.workforce_module);
                ware_configurator_panel->productionTargetUpdate();
            });
    connect(ui->sunlight_value, &QSpinBox::valueChanged,
            [this, ware_configurator_panel](int value) {
                double new_value         = static_cast<double>(value) / 100.0;
                new_value                = new_value <= 0 ? 1.0f : new_value;
                this->settings_.sunlight = new_value;

                ware_configurator_panel->productionTargetUpdate();
            });

    this->settings_.sunlight
            = static_cast<double>(ui->sunlight_value->value()) / 100;
}

WareSelectionSection::~WareSelectionSection() { delete ui; }

const common::stationbuilder::Complex &WareSelectionSection::getComplex() {
    return this->complex_;
}
