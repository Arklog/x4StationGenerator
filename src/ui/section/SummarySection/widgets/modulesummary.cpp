//
// Created by pierre on 7/20/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ModuleSummary.h" resolved

#include "modulesummary.hpp"

#include <QComboBox>
#include <QFormLayout>
#include <QScrollArea>
#include <QLabel>
#include <ui_summaryitembase.h>

#include "ui_modulesummary.h"
#include "stationbuilder/Complex.hpp"

namespace ui::section::summarysection::widgets {
    static bool cmp_name(const ModuleSummary *a, const ModuleSummary *b) {
        return false;
    };

    ModuleSummaryPriceItem::ModuleSummaryPriceItem(ModuleSummaryItemData data) :
    SummaryItemBase(),
    data{std::move(data)} {
        auto layout      = new QGridLayout(this);
        auto module_name = new QLabel{
            QString::fromStdString(fmt::format("{} x {}", data.amount, data.module.name)), this
        };
        auto module_price = new QLabel{QLocale().toString(data.module.price.avg * data.amount), this};

        layout->addWidget(module_name, 0, 0, Qt::AlignLeft);
        layout->addWidget(module_price, 0, 1, Qt::AlignRight);

        this->setLayout(layout);
    }

    bool ModuleSummaryPriceItem::Comparator::operator()(const ModuleSummaryPriceItem *a,
                                                        const ModuleSummaryPriceItem *b) {
        return a->data.module.price.avg * a->data.amount > b->data.module.price.avg * b->data.amount;
    }

    ModuleSummaryNameItem::ModuleSummaryNameItem(ModuleSummaryItemData data) :
    data{std::move(data)} {
        auto layout      = new QVBoxLayout;
        auto module_name = new QLabel(QString::fromStdString(data.module.name), this);

        layout->addWidget(module_name);

        this->setLayout(layout);
    }

    bool ModuleSummaryNameItem::Comparator::operator()(const ModuleSummaryNameItem *a, const ModuleSummaryNameItem *b) {
        return a->data.module.name < b->data.module.name;
    }

    ModuleSummaryCountItem::ModuleSummaryCountItem(ModuleSummaryItemData data) :
    data{std::move(data)} {
        auto layout        = new QGridLayout;
        auto module_name   = new QLabel(QString::fromStdString(data.module.name), this);
        auto module_amount = new QLabel(QLocale().toString(data.amount), this);

        layout->addWidget(module_name, 0, 0, Qt::AlignLeft);
        layout->addWidget(module_amount, 0, 1, Qt::AlignRight);

        this->setLayout(layout);
    }

    bool ModuleSummaryCountItem::Comparator::operator()(const ModuleSummaryCountItem *a,
                                                        const ModuleSummaryCountItem *b) {
        return a->data.amount > b->data.amount;
    }

    ModuleSummary::ModuleSummary(QWidget *parent) :
    QGroupBox("Modules", parent),
    ui(new Ui::ModuleSummary),
    layouts{},
    current_layout_index(0) {
        ui->setupUi(this);

        content_layout = new QVBoxLayout;
        content_layout->setSizeConstraint(QLayout::SetMinAndMaxSize);

        auto sort_selector  = new QFormLayout;
        auto sort_selection = new QComboBox(this);
        sort_selector->addRow(new QLabel("Sort by:", this), sort_selection);
        sort_selection->addItem("Price");
        sort_selection->addItem("Name");
        sort_selection->addItem("Count");

        auto scroll_area = new QScrollArea(this);
        auto widget      = new QWidget;
        widget->setLayout(content_layout);
        scroll_area->setWidget(widget);

        ui->layout->addLayout(sort_selector, 0, 0);
        ui->layout->addWidget(scroll_area, 1, 0);
        ui->layout->setRowStretch(1, 1);

        connect(sort_selection, &QComboBox::currentIndexChanged, [&](const int index) {
            updateDisplay_(index);
        });
    }

    ModuleSummary::~ModuleSummary() {
        delete ui;
    }

    void ModuleSummary::update(const common::stationbuilder::Complex &complex, const common::data::Store &store) {
        auto &                                        value = complex.complex;
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
        updateDisplay_(current_layout_index);
    }

    void ModuleSummary::updateDisplay_(int index) {
        switch (index) {
            case 0:
                copyToLayout(content_layout, layouts.get<0>().container);
                break;
            case 1:
                copyToLayout(content_layout, layouts.get<1>().container);
                break;
            case 2:
                copyToLayout(content_layout, layouts.get<2>().container);
                break;
            default:
                copyToLayout(content_layout, layouts.get<0>().container);
        }
    };
} // ui::section::summarysection::widgets
