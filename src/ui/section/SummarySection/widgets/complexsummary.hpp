//
// Created by pierre on 7/20/26.
//

#ifndef X4STATIONGENERATOR_SRC_UI_SECTION_SUMMARYSECTION_WIDGETS_COMPLEXSUMMARY_HPP
#define X4STATIONGENERATOR_SRC_UI_SECTION_SUMMARYSECTION_WIDGETS_COMPLEXSUMMARY_HPP

#include <QWidget>

#include "modulesummary.hpp"
#include "waresummary.hpp"
#include "stationbuilder/defines.hpp"
#include "utils/WareTargetContainer.hpp"

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


        void update(const t_x4_complex &       value, const common::utils::WareTargetContainer &ware_targets,
                    const common::data::Store &store);

    private:
        Ui::ComplexSummary *ui;

        section::summarysection::widgets::ModuleSummary *module_summary_;
        section::summarysection::widgets::WareSummary *  ware_summary_;

        void _build_modules_summary(const t_x4_complex &complex);
    };
} // ui::summarysection::widgets

#endif //X4STATIONGENERATOR_SRC_UI_SECTION_SUMMARYSECTION_WIDGETS_COMPLEXSUMMARY_HPP
