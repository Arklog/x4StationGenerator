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
