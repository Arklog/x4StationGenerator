#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "data/Store.hpp"

#include <QMainWindow>

#include "stationbuilder/Complex.hpp"
#include "stationbuilder/defines.hpp"

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
    using Store        = common::data::Store;
    using t_x4_complex = common::stationbuilder::t_x4_complex;
    using Settings     = common::stationbuilder::Settings;

    explicit MainWindow(const Store &store, QWidget *parent = nullptr);

    ~MainWindow();

public
slots:
    void exportPlan();

    void complexUpdated();

private:
    Ui::MainWindow *ui;

    WareSelectionSection *ware_selection_section_;
    DockAndPierrSection * dock_and_pierr_section_;
    StorageSection *      storage_section_;
    SummarySection *      summary_section_;
    SettingsSection *     settings_section_;

    Settings                        settings_;
    const Store &                   store_;
    common::stationbuilder::Complex complex_;
};

#endif // MAINWINDOW_H
