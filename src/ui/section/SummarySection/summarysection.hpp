//
// Created by pierre on 7/25/25.
//

#ifndef SUMMARYSECTION_HPP
#define SUMMARYSECTION_HPP

#include "data/Store.hpp"
#include "stationbuilder/defines.hpp"

#include <QWidget>
#include "widgets/complexsummary.hpp"

class QChart;
class QChartView;
class QStackedBarSeries;
QT_BEGIN_NAMESPACE
namespace Ui {
    class SummarySection;
}

QT_END_NAMESPACE

class SummarySection : public QWidget {
    Q_OBJECT

public:
    using t_module_id       = common::types::module::Module::module_id;
    using t_module_quantity = std::map<t_module_id, unsigned int>;
    using Store             = common::data::Store;
    using t_x4_complex      = common::stationbuilder::t_x4_complex;

    explicit SummarySection(const Store &store, QWidget *parent = nullptr);

    ~SummarySection() override;

    void updateTargetList(const t_x4_complex &targets);

private:
    Ui::SummarySection *ui;

    QStackedBarSeries *cost_series_;
    QChart *           cost_chart_;
    QChartView *       cost_view_;
    const Store &      store_;

    ui::summarysection::widgets::ComplexSummary *complex_summary_;

    void updateCostTab(const t_module_quantity &modules);
};

#endif // SUMMARYSECTION_HPP
