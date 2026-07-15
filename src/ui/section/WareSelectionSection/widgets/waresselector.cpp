//
// Created by pierre on 7/17/25.
//

// You may need to build the project (run Qt uic code generator) to get
// "ui_WaresSelector.h" resolved

#include "waresselector.hpp"

#include <QLabel>
#include <QPushButton>

#include "ui_waresselector.h"

#include "libcommon/data/WareModuleAndWorkforce.hpp"

#include "spdlog/spdlog.h"

WaresSelector::WaresSelector(const common::data::Store &store, QWidget *parent) :
QWidget(parent),
ui(new Ui::WaresSelector),
category_tabs{} {
    ui->setupUi(this);

    for (const auto &group: store.ware_groups.all) {
        auto widget = new QWidget(ui->categories);
        widget->setLayout(new QVBoxLayout(widget));
        widget->layout()->setAlignment(Qt::AlignTop);

        this->category_tabs[group.id] = widget;
        ui->categories->addTab(widget, QString(group.name.c_str()));
    }

    for (const auto &ware: store.wares.datas) {
        const auto &group = ware.group;
        const auto &tab   = this->category_tabs[group];
        const auto &widget
                = new QPushButton(QString(ware.name.c_str()), tab);

        connect(widget, &QPushButton::clicked, [this, &ware] {
            spdlog::info("ware {} clicked", ware.name);
            this->wareSelected(ware.id);
        });
        tab->layout()->addWidget(widget);
    }
}

WaresSelector::~WaresSelector() { delete ui; }
