//
// Created by pierre on 2/15/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ModulesSection.h" resolved

#include "modulessection.hpp"
#include "ui_modulessection.h"


ModulesSection::ModulesSection(QWidget *parent) :
        QWidget(parent), ui(new Ui::ModulesSection), _modules{}
{
    ui->setupUi(this);
    connect(ui->addModule, &QPushButton::clicked, this, &ModulesSection::addModule);
}

ModulesSection::~ModulesSection()
{
    delete ui;
}

void ModulesSection::addModule()
{
    auto module_selector = new ModuleSelectorWidget(this);
    connect(module_selector, &ModuleSelectorWidget::moduleNumberChanged, this, &ModulesSection::updateModules);
    this->ui->modules->addWidget(module_selector);
    module_selector->show();
}

void ModulesSection::updateModules(const ModuleSelectorWidget &widget)
{
    _modules.at(widget.getModule()) = widget.getModuleNumber();
    emit moduleUpdated(_modules);
}
