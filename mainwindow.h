#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widgets/stationsizewidget.h"
#include "widgets/modulessection.hpp"
#include "widgets/ressourceproducedsection.hpp"
#include "widgets/buildsummarysection.hpp"
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

    void updateModules(const t_modules &modules);

    void exportPlan();

signals:

    void generatedBuild(const StationBuilder &builder);

private:
    Ui::MainWindow    *ui;
    StationBuilder    _builder;
    StationSizeWidget *_station_size_widget;

    void updateProduction();

    void updateEndModules();

    void clearWidget(QWidget *widget);
};

#endif // MAINWINDOW_H
