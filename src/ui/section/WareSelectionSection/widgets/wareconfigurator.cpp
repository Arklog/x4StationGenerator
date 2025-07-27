//
// Created by pierre on 7/17/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_WareConfigurator.h" resolved

#include "wareconfigurator.hpp"
#include "ui_wareconfigurator.h"

#include "Data/Data.hpp"

#include "Data/WareModuleAndWorkforce.hpp"

#include "spdlog/spdlog.h"


WareConfigurator::WareConfigurator(const t_ware_id &ware_id, QWidget *parent) : QFrame(parent),
    ui(new Ui::WareConfigurator), ware_target{.ware_id = ware_id} {
    ui->setupUi(this);
    QFrame::setFrameShape(QFrame::StyledPanel);

    const auto &ware = getWares().at(ware_id);
    const auto &ware_name = ware->name;
    const auto &production_methods = ware->production;

    ui->ware_label->setText(QString(ware_name.c_str()));

    this->ware_target.production_method_id = production_methods[0].method;
    for (const auto &production_method: production_methods) {
        auto text = QString(production_method.name.c_str());
        ui->production_method_combo_box->addItem(text);
    }

    this->ware_target.prodution = ui->target_input->value();
    this->ware_target.production_method_id = getProductionMethodFromName(
        ui->production_method_combo_box->currentText().toStdString());


    auto trigger_update_target = [this](int value) -> void {
        spdlog::info("{} target value changed {}", this->ware_target.ware_id, value);
        this->ware_target.prodution = value;
        this->shouldUpdate();
    };
    auto trigger_update_production_method = [this](const QString &new_id) -> void {
        spdlog::info("{} production method changed {}", this->ware_target.ware_id, new_id.toStdString());
        const auto &production_method = getProductionMethodFromName(new_id.toStdString());
        this->ware_target.production_method_id = new_id.toStdString();
        this->shouldUpdate();
    };
    connect(ui->remove_button, &QPushButton::clicked, [this, ware_id](bool clicked) {
        spdlog::info("Removing ware {}", ware_id);
        this->shouldRemove(this->ware_target.ware_id);
    });
    connect(ui->production_method_combo_box, &QComboBox::currentTextChanged, trigger_update_production_method);
    connect(ui->target_input, &QSpinBox::valueChanged, trigger_update_target);
}

WareConfigurator::~WareConfigurator() {
    delete ui;
}

const WareTarget *WareConfigurator::getWareTarget() const {
    return &(this->ware_target);
}
