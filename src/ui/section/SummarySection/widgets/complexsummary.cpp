//
// Created by pierre on 7/20/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ComplexSummary.h" resolved

#include "complexsummary.hpp"

#include <QGroupBox>

#include "ui_complexsummary.h"
#include "modulesummary.hpp"
#include "overviewsummary.hpp"
#include "waresummary.hpp"
#include "stationbuilder/Complex.hpp"

namespace ui::summarysection::widgets {
    ComplexSummary::ComplexSummary(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComplexSummary) {
        ui->setupUi(this);

        auto layout = new QGridLayout(this);

        module_summary_   = new section::summarysection::widgets::ModuleSummary(this);
        ware_summary_     = new section::summarysection::widgets::WareSummary{this};
        overview_summary_ = new section::summarysection::widgets::OverviewSummary{this};

        layout->addWidget(module_summary_, 0, 0, 1, 1);
        layout->addWidget(ware_summary_, 0, 1, 1, 1);
        layout->addWidget(overview_summary_, 1, 0, 1, 2);
        this->setLayout(layout);
    }

    ComplexSummary::~ComplexSummary() {
        delete ui;
    }

    void ComplexSummary::update(const common::stationbuilder::Complex &complex, const common::data::Store &store) {
        // if (complex.value.empty())
        // return;
        module_summary_->update(complex, store);
        ware_summary_->update(complex, store);
        overview_summary_->update(complex, store);
    }
} // ui::summarysection::widgets
