#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Data/Store.hpp"

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

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow (const Store &store, QWidget *parent = nullptr);

    ~MainWindow ();

  public slots:
    void exportPlan ();

    void complexUpdated ();

  private:
    Ui::MainWindow *ui;

    WareSelectionSection *ware_selection_section_;
    DockAndPierrSection *dock_and_pierr_section_;
    StorageSection *storage_section_;
    SummarySection *summary_section_;
    SettingsSection *settings_section_;

    Settings settings_;
    const Store &store_;
    t_x4_complex complex_;
};

#endif // MAINWINDOW_H
