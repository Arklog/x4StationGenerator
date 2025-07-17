//
// Created by pierre on 7/17/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_WareConfigurator.h" resolved

#include "wareconfigurator.hpp"
#include "ui_wareconfigurator.h"

#include "Data/Data.hpp"


WareConfigurator::WareConfigurator(t_ware_id ware_id, QWidget *parent) :
    QWidget(parent), ui(new Ui::WareConfigurator) {
    ui->setupUi(this);
}

WareConfigurator::~WareConfigurator() {
    delete ui;
}
