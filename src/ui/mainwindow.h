#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "section/StorageSelectionSection/storagesection.hpp"

class DockAndPierrSection;
class WareSelectionSection;

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

signals:
private:
    Ui::MainWindow *ui;

    WareSelectionSection *ware_selection_section_;
    DockAndPierrSection *dock_and_pierr_section_;
    StorageSection *storage_section_;
};

#endif // MAINWINDOW_H
