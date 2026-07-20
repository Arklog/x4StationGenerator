//
// Created by pierre on 7/20/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ComplexSummary.h" resolved

#include "complexsummary.hpp"

#include "modulesummary.hpp"
#include "ui_complexsummary.h"

namespace ui::summarysection::widgets {
    ComplexSummary::ComplexSummary(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComplexSummary) {
        ui->setupUi(this);

        module_summary_ = new section::summarysection::widgets::ModuleSummary(this);
        ui->modules_summary->addWidget(module_summary_);
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
