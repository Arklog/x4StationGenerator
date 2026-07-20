//
// Created by pierre on 7/20/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ModuleSummary.h" resolved

#include "modulesummary.hpp"

#include <QFormLayout>
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

        auto layout       = new QFormLayout(this);
        auto module_name  = new QLabel{QString::fromStdString(data.module.name)};
        auto module_price = new QLabel{QString::fromStdString(std::to_string(data.module.price.avg * data.amount))};
        layout->addRow(module_name, module_price);

        this->setLayout(layout);
    }

    bool ModuleSummaryPriceItem::Comparator::operator()(const ModuleSummaryPriceItem *a,
                                                        const ModuleSummaryPriceItem *b) {
        return a->data.module.price.avg * a->data.amount < b->data.module.price.avg * b->data.amount;
    }

    ModuleSummary::ModuleSummary(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModuleSummary),
    layouts(price_layout{this}) {
        ui->setupUi(this);
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

        layouts.emplace_range(list);
        this->layout()->deleteLater();
        this->setLayout(layouts.get<0>().layout);
    }
} // ui::section::summarysection::widgets
