//
// Created by pierre on 7/27/25.
//

#ifndef SETTINGSSECTION_HPP
#define SETTINGSSECTION_HPP

#include <QWidget>

#include "stationbuilder/defines.hpp"


namespace ui::utils {
    class SharedState;
}

QT_BEGIN_NAMESPACE
namespace Ui {
    class SettingsSection;
}

QT_END_NAMESPACE

class SettingsSection : public QWidget {
    Q_OBJECT

public:
    using Settings = common::stationbuilder::Settings;

    explicit SettingsSection(ui::utils::SharedState &settings, QWidget *parent = nullptr);

    ~SettingsSection() override;

private:
    Ui::SettingsSection *ui;

    ui::utils::SharedState &settings_;
};


#endif //SETTINGSSECTION_HPP
