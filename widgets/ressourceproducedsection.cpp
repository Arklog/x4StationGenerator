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

void RessourceProducedSection::modulesUpdated(const StationBuilder &builder)
{
    t_ressources ressources = builder.getRessources();

    std::vector<std::pair<RESSOURCE, int>> sorted{};

    for (auto const &iter: ressources) {
        auto pos = std::lower_bound(
                sorted.begin(), sorted.end(), iter,
                [](auto const &a, auto const &b) { return a.second > b.second; }
        );
        sorted.insert(pos, iter);
    }

    clearLayout(ui->ressources_produced);
    for (auto const &ressource: sorted) {
        auto const name       = ressourcesNames.at(ressource.first) + ":";
        auto       name_label = new QLabel(name.c_str(), this);
        auto       qty        = new QLabel(QString("%1").arg(ressource.second), this);

        name_label->setStyleSheet("font-weight: bold;");
        if (ressource.second < 0)
            qty->setStyleSheet("color: red;");
        else
            qty->setStyleSheet("color: green;");

        ui->ressources_produced->addRow(name_label, qty);
    }
}
