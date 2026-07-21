//
// Created by pierre on 7/20/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ComplexSummary.h" resolved

#include "complexsummary.hpp"

#include <QGroupBox>

#include "modulesummary.hpp"
#include "ui_complexsummary.h"
#include "waresummary.hpp"

namespace ui::summarysection::widgets {
    ComplexSummary::ComplexSummary(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComplexSummary) {
        ui->setupUi(this);

        auto layout = new QGridLayout(this);

        module_summary_        = new section::summarysection::widgets::ModuleSummary(this);
        ware_summary_          = new section::summarysection::widgets::WareSummary{this};
        auto other_placeholder = new QGroupBox("other", this);

        layout->addWidget(module_summary_, 0, 0, 1, 1);
        layout->addWidget(ware_summary_, 0, 1, 1, 1);
        layout->addWidget(other_placeholder, 1, 0, 1, 2);
        this->setLayout(layout);
    }

    ComplexSummary::~ComplexSummary() {
        delete ui;
    }

    void ComplexSummary::update(const t_x4_complex &value, const common::utils::WareTargetContainer &ware_targets,
                                const common::data::Store &store) {
        if (value.empty())
            return;
        module_summary_->update(value, store);
        ware_summary_->update(ware_targets, store);
    }
} // ui::summarysection::widgets
