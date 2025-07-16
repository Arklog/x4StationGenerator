//
// Created by pierre on 2/16/25.
//

#ifndef X4STATIONGENERATOR_SETTINGSSECTION_HPP
#define X4STATIONGENERATOR_SETTINGSSECTION_HPP

#include <QWidget>
#include "BuildSettings.hpp"
#include "../widgets/ui_stationsizewidget.h"


QT_BEGIN_NAMESPACE
namespace Ui { class SettingsSection; }
QT_END_NAMESPACE

class SettingsSection : public QWidget {
Q_OBJECT

public:
    explicit SettingsSection(BuildSettings &settings, QWidget *parent = nullptr);

    ~SettingsSection() override;

signals:

    void settingsUpdated();

private:
    Ui::SettingsSection *ui;
    BuildSettings       &_settings;
};


#endif //X4STATIONGENERATOR_SETTINGSSECTION_HPP
