//
// Created by pierre on 7/16/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_wareselectionsection.h" resolved

#include "wareselectionsection.h"

#include <QLabel>
#include <QLayout>

#include "Data/Modules.hpp"

#include "ui/section/ui_wareselectionsection.h"
#include "Data/Wares.hpp"


WareSelectionSection::WareSelectionSection(QWidget *parent) :
    QWidget(parent), ui(new Ui::WareSelectionSection) {
    ui->setupUi(this);

    auto &wares = getWares();
    auto &modules = getModules();

    for (auto &iter: modules) {
        const auto tmp = new QLabel(iter.name.c_str(), this);
        ui->verticalLayout->addWidget(tmp);
    }
}

WareSelectionSection::~WareSelectionSection() {
    delete ui;
}
