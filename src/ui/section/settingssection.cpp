//
// Created by pierre on 2/16/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SettingsSection.h" resolved

#include <QRadioButton>
#include <QCheckBox>
#include <QSpinBox>
#include "settingssection.hpp"
#include "ui/section/ui_settingssection.h"
#include "ui/widgets/stationsizewidget.h"


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

    auto sun_factor = new QSpinBox(this);
    sun_factor->setRange(1, 2000);
    sun_factor->setValue(100);
    connect(
            sun_factor, QOverload<int>::of(&QSpinBox::valueChanged), [&](int value) {
                _settings.setSunFactor(value);
                emit settingsUpdated();
            }
    );
    ui->col_1->addRow("Sun Factor", sun_factor);

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
