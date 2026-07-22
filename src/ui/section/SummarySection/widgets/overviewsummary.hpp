//
// Created by pierre on 7/22/26.
//

#ifndef X4STATIONGENERATOR_SRC_UI_SECTION_SUMMARYSECTION_WIDGETS_OVERVIEWSUMMARY_HPP
#define X4STATIONGENERATOR_SRC_UI_SECTION_SUMMARYSECTION_WIDGETS_OVERVIEWSUMMARY_HPP

#include <QGroupBox>
#include <QWidget>
#include <QLabel>

namespace common::data {
    struct Store;
}

namespace common::stationbuilder {
    struct Complex;
}

namespace ui::section::summarysection::widgets {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class OverviewSummary;
    }

    QT_END_NAMESPACE

    class OverviewSummary : public QGroupBox {
        Q_OBJECT

    public:
        explicit OverviewSummary(QWidget *parent = nullptr);

        ~OverviewSummary() override;

        void update(const common::stationbuilder::Complex &complex, const common::data::Store &store);

    private:
        Ui::OverviewSummary *ui;

        struct Widgets {
            Widgets(QWidget *parent = nullptr);

            QLabel *station_name;
            QLabel *sun_factor;
            QLabel *station_cost;
            QLabel *wares_price_total;
            QLabel *workforce_total;
            QLabel *dock_number;
            QLabel *pier_number;
            QLabel *container_storage;
            QLabel *liquid_storage;
            QLabel *solid_storage;
        } widgets_;
    };
} // ui::section::summarysection::widgets

#endif //X4STATIONGENERATOR_SRC_UI_SECTION_SUMMARYSECTION_WIDGETS_OVERVIEWSUMMARY_HPP
