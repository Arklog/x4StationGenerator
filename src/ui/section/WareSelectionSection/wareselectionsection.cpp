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
#include "utils/SharedState.hpp"

WareSelectionSection::WareSelectionSection(ui::utils::SharedState &state,
                                           const Store &           store, QWidget *parent) :
QWidget(parent),
ui(new Ui::WareSelectionSection),
state_{state},
store_(store) {
    ui->setupUi(this);
    auto ware_selector           = new WaresSelector(store, this);
    auto ware_configurator_panel = new WareConfiguratorPanel(state, store, this);
    auto settings                = state_.settings();

    settings->workforce_module = store.habitats.datas[0].module.get().id;
    for (auto const &habitat: store.habitats.datas) {
        ui->habitat_input->addItem(QString::fromStdString(habitat.module.value().name));
    }

    ui->main_layout->addWidget(ware_selector, 0, 0);
    ui->main_layout->setColumnStretch(0, 1);

    ui->main_layout->addWidget(ware_configurator_panel, 0, 1);
    ui->main_layout->setColumnStretch(1, 1);

    ui->habitat_input->currentTextChanged(ui->habitat_input->currentText());

    connect(ware_selector, &WaresSelector::wareSelected,
            [this, ware_configurator_panel](t_ware_id ware_id) {
                ware_configurator_panel->addWare(ware_id, false, 0);
            });
    connect(ui->workforce_input, &QCheckBox::toggled,
            [this](bool checked) {
                this->state_.settings()->workforce_enables = checked;
                spdlog::debug("workforce enabled: {}", this->state_.settings()->workforce_enables);
            });
    connect(ui->habitat_input, &QComboBox::currentTextChanged,
            [this](QString text) {
                auto module                               = this->store_.modules.by_name.at(text.toStdString());
                this->state_.settings()->workforce_module = module->module.get().id;
                spdlog::debug("default habitat changed: {}", this->state_.settings()->workforce_module);
            });
    connect(ui->sunlight_value, &QSpinBox::valueChanged,
            [this](int value) {
                double new_value                  = static_cast<double>(value) / 100.0;
                new_value                         = new_value <= 0 ? 1.0f : new_value;
                this->state_.settings()->sunlight = new_value;
            });

    this->state_.settings()->sunlight = static_cast<double>(ui->sunlight_value->value()) / 100;
}

WareSelectionSection::~WareSelectionSection() { delete ui; }
