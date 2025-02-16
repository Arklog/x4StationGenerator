//
// Created by pierre on 2/16/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SettingsSection.h" resolved

#include <QRadioButton>
#include <QCheckBox>
#include "settingssection.hpp"
#include "widgets/section/ui_settingssection.h"
#include "widgets/stationsizewidget.h"


SettingsSection::SettingsSection(BuildSettings &settings, QWidget *parent) :
        QWidget(parent), ui(new Ui::SettingsSection), _settings(settings)
{
    ui->setupUi(this);

    auto workforce = new QCheckBox("Workforce", this);
    connect(
            workforce, &QCheckBox::stateChanged, [&](int state) {
                _settings.setWorkforce(state == Qt::Checked);
                emit settingsUpdated();
            }
    );
    ui->col_1->addRow(workforce);

    auto size_widget = new StationSizeWidget(this);
    connect(
            size_widget, &StationSizeWidget::sizeChanged, [&](StationSize size) {
                _settings.setSize(size);
                emit settingsUpdated();
            }
    );
    ui->col_2->addRow("Station Size", size_widget);
}

SettingsSection::~SettingsSection()
{
    delete ui;
}
