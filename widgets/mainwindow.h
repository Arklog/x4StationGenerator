#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widgets/stationsizewidget.h"
#include "section/modulessection.hpp"
#include "section/ressourceproducedsection.hpp"
#include "section/buildsummarysection.hpp"
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
    BuildSettings     _settings;
    StationBuilder    _builder;
    StationSizeWidget *_station_size_widget;
};

#endif // MAINWINDOW_H
