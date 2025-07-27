//
// Created by pierre on 7/25/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SummarySection.h" resolved

#include "summarysection.hpp"

#include "ui_summarysection.h"

#include "Data/WareModuleAndWorkforce.hpp"
#include <QLabel>
#include <QStackedBarSeries>
#include <QBarCategoryAxis>
#include <QBarSet>
#include <QChartView>

#include "spdlog/spdlog.h"

SummarySection::SummarySection(QWidget *parent) :
    QWidget(parent), ui(new Ui::SummarySection) {
    ui->setupUi(this);

    auto cost_series = new QStackedBarSeries(this);
    auto cost_chart = new QChart();
    cost_chart->addSeries(cost_series);
    auto cost_chart_view = new QChartView(cost_chart, this);

    // auto revenue_chart = new QStackedBarSeries();
    ui->cost_tab_layout->addWidget(cost_chart_view);
    cost_series_ = cost_series;

    cost_series_ = cost_series;
    cost_chart_ = cost_chart;
    cost_view_ = cost_chart_view;
}

SummarySection::~SummarySection() {
    delete ui;
}

void SummarySection::updateTargetList(const t_x4_complex &targets) {
    auto modules = getModules();
    t_module_quantity modules_recap{};

    for (const auto &target: targets)
        modules_recap[target] += 1;

    updateCostTab(modules_recap);
}

void SummarySection::updateCostTab(const t_module_quantity &modules) {
    const auto& all_modules = getModules();
    cost_series_->clear();

    // extract all wares for build
    auto categories = QStringList();

    for (const auto &[module_id, module_amount]: modules) {
        const auto& module = all_modules.at(module_id);
        const auto& wares = module->build_cost.wares;

        for (auto const &[ware_id, ware_amount]: wares) {
            auto iter = std::find(categories.cbegin(), categories.cend(), QString::fromStdString(ware_id));

            if (iter != categories.cend())
                continue;
            categories.append(QString::fromStdString(ware_id));
        }
    }

    // generate ressource map
    std::vector<QBarSet*> bar_sets{};

    for (const auto &[module_id, module_amount]: modules) {
        std::unordered_map<t_ware_id, unsigned int> ware_quantity;
        auto module = all_modules.at(module_id);
        auto bar_set = new QBarSet(QString::fromStdString(module->name));

        const auto& wares = module->build_cost.wares;
        for (auto const &[ware_id, ware_amount]: wares) {
            auto ware = getWares().at(ware_id);
            ware_quantity[ware_id] += ware_amount * module_amount * ware->price.avg;
        }

        for (auto & ware_id : categories) {
            auto string = ware_id.toStdString();
            *bar_set << ware_quantity[string];
        }

        bar_sets.push_back(bar_set);
    }

    auto serie = new QStackedBarSeries(this);
    auto chart = new QChart();
    auto axis = new QBarCategoryAxis();

    for (auto iter: bar_sets)
        serie->append(iter);

    chart->addSeries(serie);
    axis->append(categories);
    chart->addAxis(axis, Qt::AlignBottom);
    serie->attachAxis(axis);
    chart->createDefaultAxes();
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    cost_view_->setChart(chart);
}
