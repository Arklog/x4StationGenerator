//
// Created by pierre on 7/25/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SummarySection.h" resolved

#include "summarysection.hpp"

#include "ui_summarysection.h"

#include <QLabel>
#include <QStackedBarSeries>
#include <QBarCategoryAxis>
#include <QBarSet>
#include <QChartView>
#include <sstream>

#include "Data/Data.hpp"
#include "spdlog/spdlog.h"

static void clearLayout(QLayout *layout) {
    while (auto item = layout->takeAt(0)) {
        if (auto widget = item->widget()) {
            widget->deleteLater();
        }
    }
}

SummarySection::SummarySection(QWidget *parent) : QWidget(parent), ui(new Ui::SummarySection) {
    ui->setupUi(this);

    auto cost_chart_view = new QChartView(this);

    // auto revenue_chart = new QStackedBarSeries();
    ui->cost_tab_layout->setColumnStretch(0, 2);
    ui->cost_tab_layout->setColumnStretch(1, 1);
    ui->cost_tab_layout->addWidget(cost_chart_view, 0, 0);
    ui->cost_tab_layout->removeWidget(ui->modules_group);
    ui->cost_tab_layout->addWidget(ui->modules_group, 0, 1);

    cost_view_ = cost_chart_view;
}

SummarySection::~SummarySection() {
    delete ui;
}

void SummarySection::updateTargetList(const t_x4_complex &targets) {
    auto              modules = Data::modules->module_map;
    t_module_quantity modules_recap{};

    for (const auto &target: targets)
        modules_recap[target] += 1;

    updateCostTab(modules_recap);
}

void SummarySection::updateCostTab(const t_module_quantity &modules) {
    const auto &all_modules = Data::modules->module_map;
    const auto &all_wares   = Data::wares->ware_map;
    cost_view_->deleteLater();
    cost_view_ = new QChartView(this);
    ui->cost_tab_layout->addWidget(cost_view_, 0, 0);


    // extract all wares for build
    auto                           categories = QStringList();
    std::map<t_ware_id, QBarSet *> bar_sets;

    for (const auto &[module_id, module_amount]: modules) {
        const auto &module = all_modules.at(module_id);
        const auto &wares  = module->build_cost.wares;

        categories.append(QString::fromStdString(module->name));
        for (const auto &ware: wares) {
            if (bar_sets.contains(ware.id))
                continue;
            auto label        = QString::fromStdString(all_wares.at(ware.id)->name);
            bar_sets[ware.id] = new QBarSet(label);
        }
    }

    for (auto &[ware_id, bar_set]: bar_sets) {
        for (const auto &[module_id, module_amount]: modules) {
            auto module = Data::modules->module_map.at(module_id);
            auto ware   = Data::wares->ware_map.at(ware_id);

            auto production = module->getBuildCost();
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
        auto              module = all_modules.at(module_id);
        auto              price  = module->price.avg * module_amount;
        std::stringstream ss;

        ss.imbue(std::locale("en_US.UTF-8"));
        ss << module_amount << "x " << module->name;
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
