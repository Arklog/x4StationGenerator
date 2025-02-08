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
    connect(ui->exportButton, &QPushButton::clicked, this, &MainWindow::exportPlan);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::addModule() {
    auto moduleSelector = new ModuleSelector(ui->modules);
    connect(moduleSelector, &ModuleSelector::moduleNumberChanged, this, &MainWindow::updateModules);
    ui->modules->layout()->addWidget(moduleSelector);
    this->_module_selectors.push_back(moduleSelector);
}

void MainWindow::updateModules() {
    t_modules base{};

    for (auto i: this->_module_selectors) {
        base[i->getModule()] += i->getModuleNumber();
    }

    this->_builder = StationBuilder(base, false);
    this->updateProduction();
    this->updateEndModules();
}

void MainWindow::exportPlan() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);

    if (dialog.exec()) {
        QFile file(dialog.selectedFiles().first());
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            file.write(genModulePlan(ui->stationname->toPlainText().toStdString(), _builder.get()).c_str());
        }
    }
}

void MainWindow::updateProduction() {
    const auto &production = _builder.getRessources();
    clearWidget(ui->production);

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

void MainWindow::updateEndModules() {
    clearWidget(ui->summary);
    const auto &end_modules = _builder.getModulesMap();

    for (auto const &iter: end_modules) {
        auto label = new QLabel(QString("%1: %2").arg(iter.first.name.c_str()).arg(iter.second));
        label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        ui->summary->layout()->addWidget(label);
    }
}
