#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "StationBuilder/defines.hpp"

class SummarySection;
class DockAndPierrSection;
class StorageSection;
class WareSelectionSection;
class SettingsSection;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

public slots:
    void exportPlan();

    void complexUpdated();

private:
    Ui::MainWindow *ui;

    WareSelectionSection *ware_selection_section_;
    DockAndPierrSection *dock_and_pierr_section_;
    StorageSection *storage_section_;
    SummarySection *summary_section_;
    SettingsSection *settings_section_;

    Settings settings_;
    t_x4_complex complex_;
};

#endif // MAINWINDOW_H
