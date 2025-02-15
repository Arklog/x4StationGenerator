//
// Created by pierre on 2/16/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SettingsSection.h" resolved

#include "settingssection.hpp"
#include "widgets/section/ui_settingssection.h"


SettingsSection::SettingsSection(QWidget *parent) :
        QWidget(parent), ui(new Ui::SettingsSection)
{
    ui->setupUi(this);
}

SettingsSection::~SettingsSection()
{
    delete ui;
}
