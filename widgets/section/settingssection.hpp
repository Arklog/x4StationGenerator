//
// Created by pierre on 2/16/25.
//

#ifndef X4STATIONGENERATOR_SETTINGSSECTION_HPP
#define X4STATIONGENERATOR_SETTINGSSECTION_HPP

#include <QWidget>
#include "BuildSettings.hpp"


QT_BEGIN_NAMESPACE
namespace Ui { class SettingsSection; }
QT_END_NAMESPACE

class SettingsSection : public QWidget {
Q_OBJECT

public:
    explicit SettingsSection(QWidget *parent = nullptr);

    ~SettingsSection() override;

private:
    Ui::SettingsSection *ui;
};


#endif //X4STATIONGENERATOR_SETTINGSSECTION_HPP
