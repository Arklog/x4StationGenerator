//
// Created by pierre on 2/15/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_BuildSummarySection.h" resolved

#include "buildsummarysection.hpp"
#include "widgets/ui_buildsummarysection.h"


BuildSummarySection::BuildSummarySection(QWidget *parent) :
        QWidget(parent), ui(new Ui::BuildSummarySection)
{
    ui->setupUi(this);
}

BuildSummarySection::~BuildSummarySection()
{
    delete ui;
}

void BuildSummarySection::modulesUpdated(const StationBuilder &builder)
{

}
