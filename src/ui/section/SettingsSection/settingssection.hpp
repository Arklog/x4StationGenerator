//
// Created by pierre on 7/27/25.
//

#ifndef SETTINGSSECTION_HPP
#define SETTINGSSECTION_HPP

#include <QWidget>


struct Settings;
QT_BEGIN_NAMESPACE
namespace Ui { class SettingsSection; }
QT_END_NAMESPACE

class SettingsSection : public QWidget {
Q_OBJECT

public:
    explicit SettingsSection(Settings &settings, QWidget *parent = nullptr);
    ~SettingsSection() override;

private:
    Ui::SettingsSection *ui;

    Settings &settings_;
};


#endif //SETTINGSSECTION_HPP
