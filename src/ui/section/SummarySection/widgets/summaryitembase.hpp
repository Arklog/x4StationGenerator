//
// Created by pierre on 7/20/26.
//

#ifndef X4STATIONGENERATOR_SRC_UI_SECTION_SUMMARYSECTION_WIDGETS_SUMMARYITEMBASE_HPP
#define X4STATIONGENERATOR_SRC_UI_SECTION_SUMMARYSECTION_WIDGETS_SUMMARYITEMBASE_HPP

#include <QWidget>

namespace ui::section::summarysection::widgets {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class SummaryItemBase;
    }

    QT_END_NAMESPACE

    class SummaryItemBase : public QWidget {
        Q_OBJECT

    public:
        explicit SummaryItemBase(QWidget *parent = nullptr);

        ~SummaryItemBase() override;

    protected:
        Ui::SummaryItemBase *ui;
    };
} // ui::section::summarysection::widget

#endif //X4STATIONGENERATOR_SRC_UI_SECTION_SUMMARYSECTION_WIDGETS_SUMMARYITEMBASE_HPP
