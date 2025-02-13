#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "moduleselector.h"
#include "stationsizewidget.h"
#include "StationBuilder/StationBuilder.hpp"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

public slots:

    void addModule();

    void updateModules();

    void exportPlan();

private:
    Ui::MainWindow                *ui;
    std::vector<ModuleSelector *> _module_selectors;
    StationBuilder                _builder;
    StationSizeWidget             *_station_size_widget;

    void updateProduction();

    void updateEndModules();

    void clearWidget(QWidget *widget);
};

#endif // MAINWINDOW_H
