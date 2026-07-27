//
// Created by pierre on 7/27/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SettingsSection.h" resolved

#include "settingssection.hpp"
#include "ui_settingssection.h"

#include "stationbuilder/defines.hpp"
#include "spdlog/spdlog.h"

#include <QLineEdit>

#include "utils/SharedState.hpp"


SettingsSection::SettingsSection(ui::utils::SharedState &settings, QWidget *parent) :
QWidget(parent),
ui(new Ui::SettingsSection),
settings_(settings) {
    ui->setupUi(this);

    ui->station_name_input->setText(QString::fromStdString(settings.settings()->name));

    connect(ui->station_name_input, &QLineEdit::editingFinished,
            [this]() {
                spdlog::debug("settings modified");
                settings_.settings()->name = ui->station_name_input->text().toStdString();
            });
}

SettingsSection::~SettingsSection() {
    delete ui;
}
