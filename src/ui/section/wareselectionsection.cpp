//
// Created by pierre on 7/16/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_wareselectionsection.h" resolved

#include "wareselectionsection.h"

#include <QLabel>
#include <QLayout>

#include "Data/WaresAndModules.hpp"


#include "ui/section/ui_wareselectionsection.h"
#include "ui/widgets/waresselector.hpp"


WareSelectionSection::WareSelectionSection(QWidget *parent) :
    QWidget(parent), ui(new Ui::WareSelectionSection) {
    ui->setupUi(this);
    this->setLayout(new QHBoxLayout(this));

    auto ware_selector = new WaresSelector(this);
    this->layout()->addWidget(ware_selector);
}

WareSelectionSection::~WareSelectionSection() {
    delete ui;
}
