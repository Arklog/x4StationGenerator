#include <QLabel>
#include <cmath>
#include <QFileSelector>
#include <QFileDialog>
#include <QTextStream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "moduleselector.h"
#include "modules.hpp"
#include "ModuleGenerator.hpp"
#include "StationBuilder/StationBuilder.hpp"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->addModuleButton, &QPushButton::clicked, this, &MainWindow::addModule);
    connect(ui->autocompleteButton, &QPushButton::clicked, this, &MainWindow::autoComplete);
    connect(ui->exportButton, &QPushButton::clicked, this, &MainWindow::exportPlan);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::addModule() {
    auto moduleSelector = new ModuleSelector(ui->modules);
    connect(moduleSelector, &ModuleSelector::moduleNumberChanged, this, &MainWindow::updateModules);
    ui->modules->layout()->addWidget(moduleSelector);
    this->moduleSelectors.push_back(moduleSelector);
}

void MainWindow::updateModules() {
    this->_modules.clear();

    for (auto i: this->moduleSelectors) {
        this->_modules[i->getModule()] = i->getModuleNumber();
    }

    this->updateProduction();
}

void MainWindow::autoComplete() {
    t_modules modules;

    for (auto iter: moduleSelectors)
        modules[iter->getModule()] = iter->getModuleNumber();

    ModuleGenerator generator(modules);
    generator.generate();
    _modules = generator.get();

    clearWidget(ui->summary);
    for (auto const &iter: _modules) {
        auto label = new QLabel(QString("%1: %2").arg(iter.first.name.c_str()).arg(iter.second), ui->summary);
        label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        ui->summary->layout()->addWidget(label);
    }
}

void MainWindow::exportPlan() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    
    if (dialog.exec()) {
        QFile          file(dialog.selectedFiles().first());
        StationBuilder builder(_modules);
        builder.generateBuildOrder(priority_generator);

        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            file.write(genModulePlan(ui->stationname->toPlainText().toStdString(), builder.get()).c_str());
        }
    }
}

void MainWindow::updateProduction() {
    std::map<RESSOURCES, int> production;

    clearWidget(ui->production);

    for (const auto &i: this->_modules) {
        addMap(production, i.first.getTotal(i.second, true));
    }
    for (const auto &i: production) {
        auto label = new QLabel(QString("%1: %2").arg(ressourcesNames.at(i.first).c_str()).arg(i.second),
                                ui->production);
        label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        ui->production->layout()->addWidget(label);
    }
}

void MainWindow::clearWidget(QWidget *widget) {
    QLayoutItem *tmp;
    while ((tmp = widget->layout()->takeAt(0)) != nullptr) {
        if (tmp->widget() == nullptr)
            continue;
        widget->layout()->removeWidget(tmp->widget());
        delete tmp->widget();
    }
}
