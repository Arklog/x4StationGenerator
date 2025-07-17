//
// Created by pierre on 7/16/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_wareselectionsection.h" resolved

#include "wareselectionsection.h"

#include <QLabel>
#include <QLayout>

#include "Data/WaresAndModules.hpp"


#include "ui/section/ui_wareselectionsection.h"


WareSelectionSection::WareSelectionSection(QWidget *parent) :
    QWidget(parent), ui(new Ui::WareSelectionSection) {
    ui->setupUi(this);

    auto &wares = getWares();

    for (auto &iter: wares) {
        const auto& ware = iter.second;
        const auto tmp = new QLabel(ware->name.c_str(), this);
        ui->verticalLayout->addWidget(tmp);
    }
}

WareSelectionSection::~WareSelectionSection() {
    delete ui;
}
