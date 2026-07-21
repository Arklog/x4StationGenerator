//
// Created by pierre on 7/20/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SummaryItemBase.h" resolved

#include "summaryitembase.hpp"
#include "ui_summaryitembase.h"

namespace ui::section::summarysection::widgets {
    SummaryItemBase::SummaryItemBase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SummaryItemBase) {
        ui->setupUi(this);
    }

    SummaryItemBase::~SummaryItemBase() {
        delete ui;
    }
} // ui::section::summarysection::widget
