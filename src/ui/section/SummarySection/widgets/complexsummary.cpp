//
// Created by pierre on 7/20/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ComplexSummary.h" resolved

#include "complexsummary.hpp"

#include <QGroupBox>

#include "modulesummary.hpp"
#include "ui_complexsummary.h"

namespace ui::summarysection::widgets {
    ComplexSummary::ComplexSummary(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComplexSummary) {
        ui->setupUi(this);

        auto layout = new QGridLayout(this);

        module_summary_        = new section::summarysection::widgets::ModuleSummary(this);
        auto wares_placeholder = new QGroupBox("wares", this);
        auto other_placeholder = new QGroupBox("other", this);

        layout->addWidget(module_summary_, 0, 0, 1, 1);
        layout->addWidget(wares_placeholder, 0, 1, 1, 1);
        layout->addWidget(other_placeholder, 1, 0, 1, 2);
    }

    ComplexSummary::~ComplexSummary() {
        delete ui;
    }

    void ComplexSummary::update(const t_x4_complex &value, const common::data::Store &store) {
        if (value.empty())
            return;
        module_summary_->update(value, store);
    }
} // ui::summarysection::widgets
