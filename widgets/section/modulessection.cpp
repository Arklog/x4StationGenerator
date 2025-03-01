//
// Created by pierre on 2/15/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ModulesSection.h" resolved

#include <QLineEdit>
#include "modulessection.hpp"
#include "widgets/section/ui_modulessection.h"


ModulesSection::ModulesSection(BuildSettings &settings, QWidget *parent) :
        QWidget(parent), ui(new Ui::ModulesSection), _modules{}, _settings(settings)
{
    ui->setupUi(this);
    connect(ui->addModule, &QPushButton::clicked, this, &ModulesSection::addModule);
    connect(ui->stationName, &QLineEdit::textChanged, this, &ModulesSection::updateStationName);
}

ModulesSection::~ModulesSection()
{
    delete ui;
}

void ModulesSection::addModule()
{
    auto module_selector = new ModuleSelectorWidget(this);
    connect(module_selector, &ModuleSelectorWidget::moduleUpdated, this, &ModulesSection::updateModules);
    this->ui->modules->addWidget(module_selector);
    module_selector->show();
}

void ModulesSection::updateModules(const ModuleSelectorWidget &widget)
{
    auto &module = widget.getModule();
    if (module.name != widget.getFormerType()->name) {
        _modules.erase(*widget.getFormerType());
    }
    _modules[widget.getModule()] = widget.getModuleNumber();
    emit moduleUpdated(_modules);
}

void ModulesSection::updateStationName(const QString &text)
{
    _settings.setName(text.toStdString());
}

const t_modules &ModulesSection::getModules() const
{
    return _modules;
}
