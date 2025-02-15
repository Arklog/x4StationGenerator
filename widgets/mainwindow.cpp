#include <QLabel>
#include <cmath>
#include <QFileSelector>
#include <QFileDialog>
#include <QTextStream>
#include "mainwindow.h"
#include "widgets/ui_mainwindow.h"
#include "widgets/moduleselectorwidget.h"
#include "modules.hpp"
#include "ModuleGenerator.hpp"
#include "StationBuilder/StationBuilder.hpp"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow), _builder(), _settings()
{
    ui->setupUi(this);

    this->_station_size_widget = new StationSizeWidget(ui->settings_tab);
    ui->settings_tab->layout()->addWidget(this->_station_size_widget);

    auto module_section = new ModulesSection(_settings, ui->modules_tab);
    ui->module_tab_layout->addWidget(module_section, 0, 0);
    auto production_section = new RessourceProducedSection(_settings, ui->modules_tab);
    ui->module_tab_layout->addWidget(production_section, 0, 1);
    auto build_summary_section = new BuildSummarySection(ui->modules_tab);
    ui->module_tab_layout->addWidget(build_summary_section, 0, 2);

    connect(module_section, &ModulesSection::moduleUpdated, this, &MainWindow::updateModules);
    connect(this, &MainWindow::generatedBuild, build_summary_section, &BuildSummarySection::modulesUpdated);
    connect(this, &MainWindow::generatedBuild, production_section, &RessourceProducedSection::modulesUpdated);

    connect(build_summary_section, &BuildSummarySection::exportStation, this, &MainWindow::exportPlan);

    ui->module_tab_layout->setColumnStretch(0, 1);
    ui->module_tab_layout->setColumnStretch(1, 1);
    ui->module_tab_layout->setColumnStretch(2, 1);
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
            auto        data = genModulePlan(
                    _settings.getName(),
                    _builder.get(),
                    this->_station_size_widget->getSize());
            file.write(data.c_str());
        }
    }
}

void MainWindow::updateModules(const t_modules &modules)
{
    _builder.setWorkforce(true);
    _builder.setModules(modules);
    emit generatedBuild(_builder);
}