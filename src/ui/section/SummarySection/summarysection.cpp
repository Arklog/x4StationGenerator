//
// Created by pierre on 7/25/25.
//

// You may need to build the project (run Qt uic code generator) to get
// "ui_SummarySection.h" resolved

#include "summarysection.hpp"

#include "ui_summarysection.h"

#include "data/WareModuleAndWorkforce.hpp"

#include "utils/utils.hpp"

#include <QBarCategoryAxis>
#include <QBarSet>
#include <QChartView>
#include <QLabel>
#include <QStackedBarSeries>
#include <sstream>

#include <spdlog/spdlog.h>

SummarySection::SummarySection(const Store &store, QWidget *parent) :
QWidget(parent),
ui(new Ui::SummarySection),
store_(store) {
    ui->setupUi(this);

    complex_summary_ = new ui::summarysection::widgets::ComplexSummary(this);
    ui->summary_tab_layout->addWidget(complex_summary_, 0, 0, 1, 1);
    ui->summary_tab_layout->setRowStretch(0, 1);
    ui->summary_tab_layout->setColumnStretch(0, 1);

    auto cost_chart_view = new QChartView(this);

    // auto revenue_chart = new QStackedBarSeries();
    ui->cost_tab_layout->setColumnStretch(0, 2);
    ui->cost_tab_layout->setColumnStretch(1, 1);
    ui->cost_tab_layout->addWidget(cost_chart_view, 0, 0);
    ui->cost_tab_layout->removeWidget(ui->modules_group);
    ui->cost_tab_layout->addWidget(ui->modules_group, 0, 1);

    cost_view_ = cost_chart_view;
}

SummarySection::~SummarySection() { delete ui; }

void SummarySection::updateTargetList(const common::stationbuilder::Complex &complex) {
    spdlog::debug("Updating summary section");

    t_module_quantity modules_recap{};

    for (const auto &target: complex.complex)
        modules_recap[target] += 1;

    complex_summary_->update(complex, store_);
    updateCostTab(modules_recap);
}

void SummarySection::updateCostTab(const t_module_quantity &modules) {
    spdlog::debug("Updating cost tab");

    cost_view_->deleteLater();
    cost_view_ = new QChartView(this);
    ui->cost_tab_layout->addWidget(cost_view_, 0, 0);

    // extract all wares for build
    auto                                              categories = QStringList();
    std::map<common::types::Ware::ware_id, QBarSet *> bar_sets;

    for (const auto &[module_id, module_amount]: modules) {
        const auto &module = store_.modules.by_id.at(module_id)->module.get();
        const auto &wares  = module.cost;

        categories.append(QString::fromStdString(module.name));
        for (const auto &[ware_id, amount]: wares) {
            if (bar_sets.contains(ware_id))
                continue;
            auto label = QString::fromStdString(
                store_.wares.by_id.at(ware_id)->name);
            bar_sets[ware_id] = new QBarSet(label);
        }
    }

    for (auto &[ware_id, bar_set]: bar_sets) {
        for (const auto &[module_id, module_amount]: modules) {
            auto module = store_.modules.by_id.at(module_id)->module.get();
            auto ware   = store_.wares.by_id.at(ware_id);

            auto production = module.cost;
            auto cost       = production[ware_id] * module_amount * ware->price.avg;

            *bar_set << cost;
        }
    }

    auto serie = new QStackedBarSeries(this);
    auto chart = new QChart();
    auto axis  = new QBarCategoryAxis();

    for (auto [ware_id, bar_set]: bar_sets)
        serie->append(bar_set);

    chart->addSeries(serie);
    axis->append(categories);
    chart->createDefaultAxes();
    chart->addAxis(axis, Qt::AlignBottom);
    serie->attachAxis(axis);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    cost_view_->setChart(chart);

    clearLayout(ui->module_group_layout);
    unsigned int sum = 0;
    for (auto [module_id, module_amount]: modules) {
        auto              module = store_.modules.by_id.at(module_id)->module.get();
        auto              price  = module.price.avg * module_amount;
        std::stringstream ss;

        ss.imbue(std::locale("en_US.UTF-8"));
        ss << module_amount << "x " << module.name;
        auto label_string = QString::fromStdString(ss.str());
        auto label        = new QLabel(label_string);

        ss.str(std::string());
        ss << price;
        auto value_string = QString::fromStdString(ss.str());
        auto value_label  = new QLabel(value_string);

        ui->module_group_layout->addRow(label, value_label);
        sum += price;
    }
    std::stringstream ss;
    ss.imbue(std::locale("en_US.UTF-8"));
    ss << sum;

    auto label        = new QLabel("Total");
    auto value_string = QString::fromStdString(ss.str());
    auto value_label  = new QLabel(value_string);
    ui->module_group_layout->addRow(label, value_label);
}
