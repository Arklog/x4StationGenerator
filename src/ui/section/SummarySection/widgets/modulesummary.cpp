//
// Created by pierre on 7/20/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ModuleSummary.h" resolved

#include "modulesummary.hpp"

#include <QComboBox>
#include <QFormLayout>
#include <QScrollArea>
#include <ui_summaryitembase.h>

#include "ui_modulesummary.h"

namespace ui::section::summarysection::widgets {
    static bool cmp_name(const ModuleSummary *a, const ModuleSummary *b) {
        return false;
    };

    ModuleSummaryPriceItem::ModuleSummaryPriceItem(ModuleSummaryItemData data) :
    SummaryItemBase(),
    data{std::move(data)} {
        ui->text->deleteLater();
        delete this->layout();

        auto layout      = new QFormLayout(this);
        auto module_name = new QLabel{
            QString::fromStdString(fmt::format("{} x {}", data.amount, data.module.name)), this
        };
        auto module_price = new QLabel{
            QString::fromStdString(std::to_string(data.module.price.avg * data.amount)), this
        };

        layout->addRow(module_name, module_price);
        // module_name->setAlignment(Qt::AlignLeft);
        // module_price->setAlignment(Qt::AlignRight);

        this->setLayout(layout);
    }

    bool ModuleSummaryPriceItem::Comparator::operator()(const ModuleSummaryPriceItem *a,
                                                        const ModuleSummaryPriceItem *b) {
        return a->data.module.price.avg * a->data.amount < b->data.module.price.avg * b->data.amount;
    }

    ModuleSummary::ModuleSummary(QWidget *parent) :
    QGroupBox("Modules", parent),
    ui(new Ui::ModuleSummary),
    layouts(price_layout{}) {
        ui->setupUi(this);

        auto sort_selector  = new QFormLayout;
        auto sort_selection = new QComboBox(this);
        sort_selector->addRow(new QLabel("Sort by:", this), sort_selection);

        auto scroll_area = new QScrollArea(this);
        auto widget      = new QWidget(this);
        scroll_area->setWidget(widget);
        widget->setLayout(layouts.get<0>().layout);

        ui->layout->addLayout(sort_selector, 0, 0);
        ui->layout->addWidget(scroll_area, 1, 0);
        ui->layout->setRowStretch(1, 1);
    }

    ModuleSummary::~ModuleSummary() {
        delete ui;
    }

    void ModuleSummary::update(const t_x4_complex &value, const common::data::Store &store) {
        std::unordered_map<std::string, unsigned int> modules;
        std::ranges::for_each(value, [&](const auto &v) {
            modules[v] += 1;
        });

        auto list = std::vector<ModuleSummaryItemData>{};
        std::ranges::for_each(modules, [&](const auto &v) {
            auto &key    = v.first;
            auto  amount = v.second;

            ModuleSummaryItemData data{
                .module = store.modules.by_id.at(key)->module,
                .amount = amount
            };
            list.emplace_back(std::move(data));
        });

        layouts.clear();
        layouts.emplace_range(list);
    }
} // ui::section::summarysection::widgets
