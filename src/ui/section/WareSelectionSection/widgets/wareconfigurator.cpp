//
// Created by pierre on 7/17/25.
//

// You may need to build the project (run Qt uic code generator) to get
// "ui_WareConfigurator.h" resolved

#include "wareconfigurator.hpp"
#include "ui_wareconfigurator.h"

#include "Data/Data.hpp"

#include "Data/WareModuleAndWorkforce.hpp"

#include "spdlog/spdlog.h"

WareConfigurator::WareConfigurator(WareTarget *ware_target, QWidget *parent)
    : QFrame(parent), ui(new Ui::WareConfigurator), ware_target{ware_target} {
    ui->setupUi(this);
    QFrame::setFrameShape(QFrame::StyledPanel);

    const auto &ware_id = this->ware_target->ware_id;
    const auto &ware = getWares().at(ware_id);
    const auto &ware_name = ware->name;
    const auto &possible_source_modules = getModules(ware_id);

    ui->ware_label->setText(QString(ware_name.c_str()));

    // Create combobox of possible modules
    for (const auto &[module_id, module]: possible_source_modules) {
        auto text = QString::fromStdString(module->name);
        ui->production_method_combo_box->addItem(text);
    }

    // this->ware_target->prodution = ui->target_input->value();
    // this->ware_target->source_module = getModuleIdFromName(
    // ui->production_method_combo_box->currentText().toStdString());

    // Is triggered when the ware amount required is changed
    auto trigger_update_target = [this](int value) -> void {
        spdlog::info("{} target value changed {}",
                     this->ware_target->ware_id.raw(), value);
        this->ware_target->prodution = value;
        this->shouldUpdate();
    };

    // Is triggered when the source module is changed
    auto trigger_update_source_module = [this](const QString &new_id) -> void {
        const auto &production_method = getModuleIdFromName(new_id.toStdString());
        spdlog::info("{} production method changed {}", this->ware_target->ware_id.raw(),
                     production_method);
        this->ware_target->source_module = production_method;
        this->shouldUpdate();
    };

    connect(ui->remove_button, &QPushButton::clicked,
            [this, ware_id](bool clicked) {
                spdlog::info("Removing ware {}", ware_id.raw());
                this->shouldRemove(this->ware_target->ware_id);
            });
    connect(ui->production_method_combo_box, &QComboBox::currentTextChanged,
            trigger_update_source_module);
    connect(ui->target_input, &QSpinBox::valueChanged, trigger_update_target);

    // in case of secondary ware, we gray out the widget and disable input
    // except for the source module selection
    if (!ware_target->is_secondary)
        return;
    auto palete = this->palette();
    palete.setColor(QPalette::Window, QColor(200, 200, 200));
    this->setAutoFillBackground(true);
    this->setPalette(palete);

    ui->target_input->setReadOnly(true);
    ui->target_input->setDisabled(true);
    ui->remove_button->setDisabled(true);
    ui->target_input->setValue(ware_target->prodution);
}

WareConfigurator::~WareConfigurator() { delete ui; }

const WareTarget *WareConfigurator::getWareTarget() const {
    return this->ware_target;
}
