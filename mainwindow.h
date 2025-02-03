#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "moduleselector.h"

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

    void autoComplete();

    void exportPlan();

private:
    Ui::MainWindow                *ui;
    std::vector<ModuleSelector *> moduleSelectors;
    t_modules                     _modules;

    void updateProduction();
    void clearWidget(QWidget *widget);
};

#endif // MAINWINDOW_H
