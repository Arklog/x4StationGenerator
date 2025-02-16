#include <QLabel>
#include <cmath>
#include <QFileSelector>
#include <QFileDialog>
#include <QTextStream>
#include "mainwindow.h"
#include "widgets/ui_mainwindow.h"
#include "modules.hpp"
#include "section/settingssection.hpp"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow), _settings(), _builder(_settings)
{
    ui->setupUi(this);

    auto module_section = new ModulesSection(_settings, ui->modules_tab);
    ui->module_tab_layout->addWidget(module_section, 0, 0);
    auto production_section = new RessourceProducedSection(_settings, ui->modules_tab);
    ui->module_tab_layout->addWidget(production_section, 0, 1);
    auto build_summary_section = new BuildSummarySection(ui->modules_tab);
    ui->module_tab_layout->addWidget(build_summary_section, 0, 2);

    ui->module_tab_layout->setColumnStretch(0, 1);
    ui->module_tab_layout->setColumnStretch(1, 1);
    ui->module_tab_layout->setColumnStretch(2, 1);

    auto settings_section = new SettingsSection(_settings, ui->settings_tab);
    ui->settings_tab->layout()->addWidget(settings_section);

    connect(module_section, &ModulesSection::moduleUpdated, this, &MainWindow::updateModules);
    connect(build_summary_section, &BuildSummarySection::exportStation, this, &MainWindow::exportPlan);
    connect(
            settings_section, &SettingsSection::settingsUpdated, [this, module_section]() {
                this->updateModules(module_section->getModules());
            }
    );


    connect(this, &MainWindow::generatedBuild, build_summary_section, &BuildSummarySection::modulesUpdated);
    connect(this, &MainWindow::generatedBuild, production_section, &RessourceProducedSection::modulesUpdated);
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
                    _settings.getSize()
            );

            file.write(data.c_str());
        }
    }
}

void MainWindow::updateModules(const t_modules &modules)
{
    _builder.setModules(modules);
    emit generatedBuild(_builder);
}