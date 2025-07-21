//
// Created by pierre on 7/17/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_WareConfiguratorPanel.h" resolved

#include "wareconfiguratorpanel.hpp"

#include <QVBoxLayout>

#include "ui_wareconfiguratorpanel.h"
#include "wareconfigurator.hpp"

#include "StationBuilder/Generator/ComplexGeneratorBase.hpp"


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
    this->ware_targets.push_back(ware_configurator->getWareTarget());
    this->layout()->addWidget(ware_configurator);

    connect(ware_configurator, &WareConfigurator::shouldRemove, [this] (t_ware_id wid) -> void {
        auto widget = this->ware_configurators[wid];

        this->ware_configurators.erase(wid);
        for (auto iter = this->ware_targets.begin(); iter != this->ware_targets.end(); ++iter) {
            if (*iter == widget->getWareTarget()) {
                this->ware_targets.erase(iter);
                break;
            }
        }
        this->layout()->removeWidget(widget);

        delete widget;
    });
    connect(ware_configurator, &WareConfigurator::shouldUpdate, this, &WareConfiguratorPanel::productionTargetUpdate);
}

void WareConfiguratorPanel::productionTargetUpdate() {
    ComplexGeneratorBase test(this->ware_targets);
    test.build();
}
