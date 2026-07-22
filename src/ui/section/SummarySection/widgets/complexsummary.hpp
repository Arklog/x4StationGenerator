//
// Created by pierre on 7/20/26.
//

#ifndef X4STATIONGENERATOR_SRC_UI_SECTION_SUMMARYSECTION_WIDGETS_COMPLEXSUMMARY_HPP
#define X4STATIONGENERATOR_SRC_UI_SECTION_SUMMARYSECTION_WIDGETS_COMPLEXSUMMARY_HPP

#include <QWidget>

#include "stationbuilder/defines.hpp"

namespace ui::section::summarysection::widgets {
    class OverviewSummary;
    class WareSummary;
    class ModuleSummary;
}

namespace common::data {
    struct Store;
}

namespace common::stationbuilder {
    struct Complex;
}

namespace ui::summarysection::widgets {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class ComplexSummary;
    }

    QT_END_NAMESPACE

    class ComplexSummary : public QWidget {
        Q_OBJECT

    public:
        using t_x4_complex = common::stationbuilder::t_x4_complex;

        explicit ComplexSummary(QWidget *parent = nullptr);

        ~ComplexSummary() override;


        void update(const common::stationbuilder::Complex &complex, const common::data::Store &store);

    private:
        Ui::ComplexSummary *ui;

        section::summarysection::widgets::ModuleSummary *  module_summary_;
        section::summarysection::widgets::WareSummary *    ware_summary_;
        section::summarysection::widgets::OverviewSummary *overview_summary_;

        void _build_modules_summary(const t_x4_complex &complex);
    };
} // ui::summarysection::widgets

#endif //X4STATIONGENERATOR_SRC_UI_SECTION_SUMMARYSECTION_WIDGETS_COMPLEXSUMMARY_HPP
