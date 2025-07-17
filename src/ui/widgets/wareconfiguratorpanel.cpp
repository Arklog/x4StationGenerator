//
// Created by pierre on 7/17/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_WareConfiguratorPanel.h" resolved

#include "wareconfiguratorpanel.hpp"

#include <QVBoxLayout>

#include "ui_wareconfiguratorpanel.h"
#include "wareconfigurator.hpp"


WareConfiguratorPanel::WareConfiguratorPanel(QWidget *parent) :
    QWidget(parent), ui(new Ui::WareConfiguratorPanel) {
    ui->setupUi(this);
    this->setLayout(new QVBoxLayout());
}

WareConfiguratorPanel::~WareConfiguratorPanel() {
    delete ui;
}

void WareConfiguratorPanel::addWare(t_ware_id ware_id) {
    auto ware_configurator = new WareConfigurator(ware_id, this);
    this->layout()->addWidget(ware_configurator);
}