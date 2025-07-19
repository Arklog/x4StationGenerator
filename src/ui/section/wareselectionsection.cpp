//
// Created by pierre on 7/16/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_wareselectionsection.h" resolved

#include "wareselectionsection.h"

#include "ui/section/ui_wareselectionsection.h"
#include "ui/widgets/wareconfiguratorpanel.hpp"
#include "ui/widgets/waresselector.hpp"


WareSelectionSection::WareSelectionSection(QWidget *parent) : QWidget(parent), ui(new Ui::WareSelectionSection) {
    ui->setupUi(this);
    auto layout = new QGridLayout(this);
    auto ware_selector = new WaresSelector(this);
    auto ware_configurator_panel = new WareConfiguratorPanel(this);

    this->setLayout(layout);
    layout->setColumnStretch(0, 2);
    layout->setColumnStretch(1, 1);
    layout->addWidget(ware_selector, 0, 0);
    layout->addWidget(ware_configurator_panel, 0, 1);

    connect(ware_selector, &WaresSelector::wareSelected, ware_configurator_panel, &WareConfiguratorPanel::addWare);
}

WareSelectionSection::~WareSelectionSection() {
    delete ui;
}
