#include "moduleselector.h"
#include "ui_moduleselector.h"
#include <QString>

ModuleSelector::ModuleSelector(QWidget *parent)
        : QWidget(parent), ui(new Ui::ModuleSelector)
{
    ui->setupUi(this);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    for (const auto &item: MODULES::MODULES)
    {
        ui->comboBox->addItem(QString(item.name.c_str()));
    }

    connect(
            ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this]() {
                emit moduleNumberChanged();
            }
    );
    connect(
            ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this]() {
                emit moduleNumberChanged();
            }
    );
}

ModuleSelector::~ModuleSelector()
{
    delete ui;
}

const Module &ModuleSelector::getModule() const
{
    for (const auto &iter: MODULES::MODULES)
    {
        if (iter.name == ui->comboBox->currentText().toStdString())
        {
            return iter;
        }
    }
    throw std::runtime_error("Module not found");
    return ressourcesMap.begin()->second;
}

int ModuleSelector::getModuleNumber() const
{
    return this->ui->spinBox->value();
}

std::map<RESSOURCE, int> ModuleSelector::getProduction() const
{
    auto                     module = this->getModule();
    std::map<RESSOURCE, int> production;

    for (auto iter: module.ressources_produced)
    {
        production[iter.first] = iter.second * this->getModuleNumber();
    }

    return production;
}

std::map<RESSOURCE, int> ModuleSelector::getConsumption() const
{
    auto                     module = this->getModule();
    std::map<RESSOURCE, int> consumption;

    for (auto iter: module.ressources_consumed)
    {
        consumption[iter.first] = iter.second * this->getModuleNumber();
    }

    return consumption;
}
