//
// Created by pierre on 7/20/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ComplexSummary.h" resolved

#include "complexsummary.hpp"
#include "ui_complexsummary.h"

namespace ui::summarysection::widgets {
    ComplexSummary::ComplexSummary(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComplexSummary) {
        ui->setupUi(this);
    }

    ComplexSummary::~ComplexSummary() {
        delete ui;
    }
} // ui::summarysection::widgets
