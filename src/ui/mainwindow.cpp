#include <cmath>
#include <QFileDialog>
#include <QTextStream>
#include "mainwindow.h"

#include "modules.hpp"

#include "section/WareSelectionSection/wareselectionsection.h"

#include "ui_mainwindow.h"

#include "section/DockAndPierrSection/dockandpierrsection.hpp"

#include "spdlog/spdlog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    auto ware_selection_section = new WareSelectionSection(this);
    auto dock_and_pierr_section = new DockAndPierrSection(this);

    ui->ware_selection_tab_layout->addWidget(ware_selection_section);
    ui->dock_and_pierr_tab_layout->addWidget(dock_and_pierr_section);

    this->ware_selection_section_ = ware_selection_section;
    this->dock_and_pierr_section_ = dock_and_pierr_section;

    connect(ui->action_export, &QAction::triggered, this, &MainWindow::exportPlan);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::exportPlan() {
    spdlog::info("Exporting plan");

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);

    auto built_complex = this->ware_selection_section_->getComplex();
    auto dock_and_pierr = this->dock_and_pierr_section_->getModuleTargetList();
    t_x4_complex real_complex{};

    for (const auto &dock_or_pierr: dock_and_pierr) {
        for (size_t i = 0; i < dock_or_pierr.amount; ++i)
            real_complex.push_back(dock_or_pierr.module_id);
    }
    real_complex.insert(real_complex.end(), built_complex.begin(), built_complex.end());

    if (dialog.exec()) {
        auto selected_file = dialog.selectedFiles().first();
        spdlog::info("selected file: {0}", selected_file.toStdString());

        QFile file(selected_file);

        // non empty file, check with user
        // if (file.size() != 0) {
            // spdlog::info("non empty file, waiting for confirmation");
            // QDialog dialog(this);
            // dialog.setWindowFlags(Qt::Dialog);
            // dialog.setModal(true);
            // dialog.setWindowTitle("Non empty file");
            // dialog.exec();

            // bool ok = false;
            // connect(dialog, &QDialog::accept, [&ok] () -> void { ok = true; });

            // spdlog::info("file was accepted");
        // }

        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            auto data = genModulePlan(
                "tmp",
                real_complex
            );

            file.write(data.c_str());
        }
    }
}

// void MainWindow::updateModules(const t_modules &modules)
// {
// _builder.setModules(modules);
// emit generatedBuild(_builder);
// }
