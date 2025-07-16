//
// Created by pierre on 7/16/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_wareselectionsection.h" resolved

#include "wareselectionsection.h"
#include "ui/section/ui_wareselectionsection.h"


wareselectionsection::wareselectionsection(QWidget *parent) :
    QWidget(parent), ui(new Ui::wareselectionsection) {
    ui->setupUi(this);
}

wareselectionsection::~wareselectionsection() {
    delete ui;
}
