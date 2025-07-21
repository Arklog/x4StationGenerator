#include <cmath>
#include <QFileDialog>
#include <QTextStream>
#include "mainwindow.h"

#include "ui/section/wareselectionsection.h"

#include "ui/widgets/ui_mainwindow.h"
#include "modules.hpp"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto module_section = new WareSelectionSection(this);
    ui->module_tab_layout->addWidget(module_section, 0, 0);

    ui->module_tab_layout->setColumnStretch(0, 1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::exportPlan()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);

    if (dialog.exec()) {
        QFile file(dialog.selectedFiles().first());

        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            // auto        data = genModulePlan(
                    // _settings.getName(),
                    // _builder.get(),
                    // _settings.getSize()
            // );

            // file.write(data.c_str());
        }
    }
}

// void MainWindow::updateModules(const t_modules &modules)
// {
    // _builder.setModules(modules);
    // emit generatedBuild(_builder);
// }