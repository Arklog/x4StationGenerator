//
// Created by pierre on 2/9/25.
//

#ifndef X4STATIONGENERATOR_STATIONSIZEWIDGET_HPP
#define X4STATIONGENERATOR_STATIONSIZEWIDGET_HPP

#include <QWidget>
#include "defines.hpp"

namespace Ui {
    class StationSizeWidget;
}

class StationSizeWidget : public QWidget {
Q_OBJECT
public:
    explicit StationSizeWidget(QWidget *parent = nullptr);

    StationSize getSize();

public slots:

    void updateLabels();

signals:

    void sizeChanged(StationSize size);

private:
    Ui::StationSizeWidget *ui;
};


#endif //X4STATIONGENERATOR_STATIONSIZEWIDGET_HPP
