#include "moduleselectorwidget.h"
#include "widgets/widgets/ui_moduleselectorwidget.h"
#include <QString>

ModuleSelectorWidget::ModuleSelectorWidget(QWidget *parent)
        : QWidget(parent), ui(new Ui::ModuleSelectorWidget)
{
    ui->setupUi(this);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    for (const auto &item: MODULES::MODULES) {
        ui->comboBox->addItem(QString(item.name.c_str()));
    }

    connect(
            ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this]() {
                emit moduleNumberChanged(*this);
            }
    );
    connect(
            ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this]() {
                emit moduleNumberChanged(*this);
            }
    );
}

ModuleSelectorWidget::~ModuleSelectorWidget()
{
    delete ui;
}

const Module &ModuleSelectorWidget::getModule() const
{
    for (const auto &iter: MODULES::MODULES) {
        if (iter.name == ui->comboBox->currentText().toStdString()) {
            return iter;
        }
    }
    throw std::runtime_error("Module not found");
    return ressourcesMap.begin()->second;
}

int ModuleSelectorWidget::getModuleNumber() const
{
    return this->ui->spinBox->value();
}

std::map<RESSOURCE, int> ModuleSelectorWidget::getProduction() const
{
    auto                     module = this->getModule();
    std::map<RESSOURCE, int> production;

    for (auto iter: module.ressources_produced) {
        production[iter.first] = iter.second * this->getModuleNumber();
    }

    return production;
}

std::map<RESSOURCE, int> ModuleSelectorWidget::getConsumption() const
{
    auto                     module = this->getModule();
    std::map<RESSOURCE, int> consumption;

    for (auto iter: module.ressources_consumed) {
        consumption[iter.first] = iter.second * this->getModuleNumber();
    }

    return consumption;
}
