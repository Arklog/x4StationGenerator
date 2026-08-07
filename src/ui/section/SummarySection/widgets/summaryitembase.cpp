//
// Created by pierre on 7/20/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SummaryItemBase.h" resolved

#include "summaryitembase.hpp"
#include "ui_summaryitembase.h"

static auto create_palette = []() {
    QPalette palette;

    palette.setColor(QPalette::WindowText, Qt::red);
    return palette;
};

namespace ui::section::summarysection::widgets {
    SummaryItemBase::SummaryItemBase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SummaryItemBase) {
        ui->setupUi(this);
    }

    SummaryItemBase::~SummaryItemBase() {
        delete ui;
    }

    QPalette SummaryItemBase::palette_warn{create_palette()};
} // ui::section::summarysection::widget
