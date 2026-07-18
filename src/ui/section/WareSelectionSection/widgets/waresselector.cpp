//
// Created by pierre on 7/17/25.
//

// You may need to build the project (run Qt uic code generator) to get
// "ui_WaresSelector.h" resolved

#include "waresselector.hpp"

#include <QLabel>
#include <QPushButton>

#include "ui_waresselector.h"
#include "extractor/databuilder/ModuleAggregator.hpp"

#include "libcommon/data/WareModuleAndWorkforce.hpp"

#include "spdlog/spdlog.h"

WaresSelector::WaresSelector(const common::data::Store &store, QWidget *parent) :
QWidget(parent),
ui(new Ui::WaresSelector),
category_tabs{} {
    ui->setupUi(this);
    auto any_ware_produced = [](const std::vector<common::types::Ware *> &wares) {
        return std::ranges::any_of(wares, [](const common::types::Ware *w) {
            return w->produced;
        });
    };


    for (const auto &[group_id, items]: store.wares.by_waregroup) {
        if (items.empty() || !any_ware_produced(items))
            continue;
        
        auto widget = new QWidget(ui->categories);
        widget->setLayout(new QVBoxLayout(widget));
        widget->layout()->setAlignment(Qt::AlignTop);

        this->category_tabs[group_id] = widget;
        ui->categories->addTab(widget, QString::fromStdString(items[0]->group_name));
        populateGroup(widget, items);
    }
}

WaresSelector::~WaresSelector() { delete ui; }

void WaresSelector::populateGroup(QWidget *group_widget, const std::vector<common::types::Ware *> &wares) {
    for (const auto ware: wares) {
        const auto widget = new QPushButton(QString::fromStdString(ware->name), group_widget);

        connect(widget, &QPushButton::clicked, [this, ware] {
            spdlog::info("ware {} clicked", ware->name);
            this->wareSelected(ware->id);
        });
        group_widget->layout()->addWidget(widget);
    }
}
