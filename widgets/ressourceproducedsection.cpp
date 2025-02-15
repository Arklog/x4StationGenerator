//
// Created by pierre on 2/15/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RessourceProducedSection.h" resolved

#include "ressourceproducedsection.hpp"
#include "widgets/ui_ressourceproducedsection.h"


RessourceProducedSection::RessourceProducedSection(QWidget *parent) :
        QWidget(parent), ui(new Ui::RessourceProducedSection)
{
    ui->setupUi(this);
}

RessourceProducedSection::~RessourceProducedSection()
{
    delete ui;
}

void RessourceProducedSection::modulesUpdated(const t_modules &modules)
{
    t_ressources ressources{};

    for (auto const &module: modules)
    {
        auto tmp = module.first.getTotal(module.second, true); // TODO: add workforce
        addMap(ressources, tmp);
    }

    for (auto const &ressource: ressources)
    {
        auto const name       = ressourcesNames.at(ressource.first) + ":";
        auto       name_label = new QLabel(name.c_str(), this);
        auto       qty        = new QLabel(QString("%1").arg(ressource.second), this);

        ui->ressources_produced->addRow(name_label, qty);
    }
}
