#include <QLabel>
#include <cmath>
#include <QFileSelector>
#include <QFileDialog>
#include <QTextStream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgets/moduleselectorwidget.h"
#include "modules.hpp"
#include "ModuleGenerator.hpp"
#include "StationBuilder/StationBuilder.hpp"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    connect(ui->exportButton, &QPushButton::clicked, this, &MainWindow::exportPlan);

    this->_station_size_widget = new StationSizeWidget(ui->settings_tab);
    ui->settings_tab->layout()->addWidget(this->_station_size_widget);

    auto module_section = new ModulesSection(ui->modules_tab);
    ui->module_tab_layout->addWidget(module_section, 0, 0);
    auto production_section = new RessourceProducedSection(ui->modules_tab);
    ui->module_tab_layout->addWidget(production_section, 0, 1);
    auto build_summary_section = new BuildSummarySection(ui->modules_tab);
    ui->module_tab_layout->addWidget(build_summary_section, 0, 2);

    connect(
            module_section,
            &ModulesSection::moduleUpdated,
            build_summary_section,
            &BuildSummarySection::modulesUpdated
    );
    connect(
            module_section,
            &ModulesSection::moduleUpdated,
            production_section,
            &RessourceProducedSection::modulesUpdated
    );

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

    if (dialog.exec())
    {
        QFile file(dialog.selectedFiles().first());

        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&file);
            auto        data = genModulePlan(
                    "tmp", // TODO: get the station name
                    _builder.get(),
                    this->_station_size_widget->getSize());
            file.write(data.c_str());
        }
    }
}

//void MainWindow::updateProduction()
//{
//    const auto &production = _builder.getRessources();
//    clearWidget(ui->production);
//
//    for (const auto &i: production)
//    {
//        auto label = new QLabel(
//                QString("%1: %2").arg(ressourcesNames.at(i.first).c_str()).arg(i.second),
//                ui->production
//        );
//        label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
//        ui->production->layout()->addWidget(label);
//    }
//}

void MainWindow::clearWidget(QWidget *widget)
{
    QLayoutItem *tmp;
    while ((tmp = widget->layout()->takeAt(0)) != nullptr)
    {
        if (tmp->widget() == nullptr)
            continue;
        widget->layout()->removeWidget(tmp->widget());
        delete tmp->widget();
    }
}

//void MainWindow::updateEndModules()
//{
//    clearWidget(ui->summary);
//    const auto &end_modules = _builder.getModulesMap();
//
//    for (auto const &iter: end_modules)
//    {
//        auto label = new QLabel(QString("%1: %2").arg(iter.first.name.c_str()).arg(iter.second));
//        label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
//        ui->summary->layout()->addWidget(label);
//    }
//}
