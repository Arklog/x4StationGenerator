//
// Created by pierre on 7/25/25.
//

#ifndef SUMMARYSECTION_HPP
#define SUMMARYSECTION_HPP

#include "Data/Store.hpp"

#include <QWidget>

#include "StationBuilder/defines.hpp"
class QChart;
class QChartView;
class QStackedBarSeries;
QT_BEGIN_NAMESPACE
namespace Ui {
    class SummarySection;
}
QT_END_NAMESPACE

class SummarySection : public QWidget
{
    Q_OBJECT
    using t_module_quantity = std::map<t_module_id, unsigned int>;

  public:
    explicit SummarySection (const Store &store, QWidget *parent = nullptr);
    ~SummarySection () override;

    void updateTargetList (const t_x4_complex &targets);

  private:
    Ui::SummarySection *ui;

    QStackedBarSeries *cost_series_;
    QChart *cost_chart_;
    QChartView *cost_view_;
    const Store &store_;

    void updateCostTab (const t_module_quantity &modules);
};

#endif // SUMMARYSECTION_HPP
