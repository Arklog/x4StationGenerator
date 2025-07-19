//
// Created by pierre on 7/17/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_WareConfiguratorPanel.h" resolved

#include "wareconfiguratorpanel.hpp"

#include <QVBoxLayout>

#include "ui_wareconfiguratorpanel.h"
#include "wareconfigurator.hpp"


WareConfiguratorPanel::WareConfiguratorPanel(QWidget *parent) :
    QFrame(parent), ui(new Ui::WareConfiguratorPanel) {
    ui->setupUi(this);
    QFrame::setFrameShape(QFrame::StyledPanel);

    auto layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    this->setLayout(layout);
}

WareConfiguratorPanel::~WareConfiguratorPanel() {
    delete ui;
}

void WareConfiguratorPanel::addWare(t_ware_id ware_id) {
    if (this->ware_configurators.contains(ware_id))
        return ;

    auto ware_configurator = new WareConfigurator(ware_id, this);

    this->ware_configurators[ware_id] = ware_configurator;
    this->layout()->addWidget(ware_configurator);
}