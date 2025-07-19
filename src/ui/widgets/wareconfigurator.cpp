//
// Created by pierre on 7/17/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_WareConfigurator.h" resolved

#include "wareconfigurator.hpp"
#include "ui_wareconfigurator.h"

#include "Data/Data.hpp"
#include "Data/WaresAndModules.hpp"

#include "spdlog/spdlog.h"


WareConfigurator::WareConfigurator(const t_ware_id& ware_id, QWidget *parent) :
    QFrame(parent), ui(new Ui::WareConfigurator), ware_id(ware_id) {
    ui->setupUi(this);
    QFrame::setFrameShape(QFrame::StyledPanel);

    const auto& ware = getWares().at(ware_id);
    const auto& ware_name = ware->name;
    const auto& production_methods = ware->production;

    ui->ware_label->setText(QString(ware_name.c_str()));

    for (const auto& production_method : production_methods) {
        auto text = QString(production_method.name.c_str());
        ui->production_method_combo_box->addItem(text);
    }

    connect(ui->remove_button, &QPushButton::clicked, [this, ware_id] (bool clicked) {
        spdlog::info("Removing ware {}", ware_id);
        this->shouldRemove(this->ware_id);
    });
}

WareConfigurator::~WareConfigurator() {
    delete ui;
}
