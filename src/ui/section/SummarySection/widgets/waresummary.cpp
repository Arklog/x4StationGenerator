//
// Created by pierre on 7/21/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_WareSummary.h" resolved

#include "waresummary.hpp"

#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>

#include "ui_waresummary.h"
#include "stationbuilder/Complex.hpp"

namespace ui::section::summarysection::widgets {
    WareSummaryNameItem::WareSummaryNameItem(WareSummaryItemData data_) :
    SummaryItemBase(),
    data{std::move(data_)} {
        auto layout = new QVBoxLayout(this);
        auto text   = new QLabel{QString::fromStdString(this->data.ware.name), this};

        layout->addWidget(text);
        this->setLayout(layout);
    }

    bool WareSummaryNameItem::Comparator::operator()(const WareSummaryNameItem *a, const WareSummaryNameItem *b) {
        return a->data.ware.name < b->data.ware.name;
    }

    WareSummaryPriceItem::WareSummaryPriceItem(WareSummaryItemData data_) :
    SummaryItemBase(),
    data{std::move(data_)},
    avg_price{static_cast<long long>(data.ware.price.avg) * data.amount} {
        auto layout = new QGridLayout(this);

        auto name  = new QLabel{QString::fromStdString(data.ware.name), this};
        auto price = new QLabel{QLocale().toString(avg_price), this};

        layout->addWidget(name, 0, 0, Qt::AlignLeft);
        layout->addWidget(price, 0, 1, Qt::AlignRight);
        this->setLayout(layout);
    }

    bool WareSummaryPriceItem::Comparator::operator()(const WareSummaryPriceItem *a, const WareSummaryPriceItem *b) {
        return a->avg_price > b->avg_price;
    }

    WareSummaryAmountItem::WareSummaryAmountItem(WareSummaryItemData data_) :
    SummaryItemBase(),
    data{std::move(data_)} {
        auto layout = new QGridLayout(this);

        auto name   = new QLabel{QString::fromStdString(data.ware.name), this};
        auto amount = new QLabel{QLocale().toString(data.amount), this};

        layout->addWidget(name, 0, 0, Qt::AlignLeft);
        layout->addWidget(amount, 0, 1, Qt::AlignRight);
        this->setLayout(layout);
    }

    bool WareSummaryAmountItem::Comparator::operator()(const WareSummaryAmountItem *a, const WareSummaryAmountItem *b) {
        return a->data.amount > b->data.amount;
    }

    WareSummary::WareSummary(QWidget *parent) :
    QGroupBox("Wares", parent),
    ui(new Ui::WareSummary),
    layouts{},
    layout(new QVBoxLayout),
    current_index(0) {
        ui->setupUi(this);

        layout->setSizeConstraint(QLayout::SetMinAndMaxSize);

        auto form     = new QFormLayout();
        auto combobox = new QComboBox(this);
        auto label    = new QLabel("Sort by:", this);
        form->addRow(label, combobox);
        combobox->addItems({"Price", "Amount", "Name"});

        auto scroll_area = new QScrollArea(this);
        auto widget      = new QWidget(this);
        widget->setLayout(layout);
        scroll_area->setWidget(widget);

        auto main_layout = new QGridLayout{this};
        main_layout->addLayout(form, 0, 0);
        main_layout->addWidget(scroll_area, 1, 0);
        this->setLayout(main_layout);

        connect(combobox, &QComboBox::currentIndexChanged, [&](int index) {
            current_index = index;
            updateDisplay_();
        });

        updateDisplay_();
    }

    WareSummary::~WareSummary() {
        delete ui;
    }

    void WareSummary::update(const common::stationbuilder::Complex &complex, const common::data::Store &store) {
        auto &                           ware_targets = complex.wares;
        std::vector<WareSummaryItemData> data;

        std::ranges::for_each(ware_targets.getTargets(), [&](auto &item) {
            WareSummaryItemData itemData{};
            if (item.prodution == 0)
                return;

            itemData.ware   = *store.wares.by_id.at(item.ware_id);
            itemData.amount = item.prodution;

            data.push_back(std::move(itemData));
        });

        layouts.clear();
        layouts.emplace_range(data);
        updateDisplay_();
    }

    void WareSummary::updateDisplay_() {
        switch (current_index) {
            case 0:
                copyToLayout(layout, layouts.get<0>().container);
                break;
            case 1:
                copyToLayout(layout, layouts.get<1>().container);
                break;
            case 2:
                copyToLayout(layout, layouts.get<2>().container);
                break;
            default:
                copyToLayout(layout, layouts.get<0>().container);
        }
    }
} // ui::section::summarysection::widgets
