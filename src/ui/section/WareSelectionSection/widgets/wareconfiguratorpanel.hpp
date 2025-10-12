//
// Created by pierre on 7/17/25.
//

#ifndef WARECONFIGURATORPANEL_HPP
#define WARECONFIGURATORPANEL_HPP

#include <QFrame>

#include "ui_wareconfigurator.h"

#include "Data/Data.hpp"

#include "StationBuilder/defines.hpp"
#include "utils/WareTargetContainer.hpp"

#include <QGroupBox>

QT_BEGIN_NAMESPACE

namespace Ui {
    class WareConfiguratorPanel;
}

QT_END_NAMESPACE

class WareConfigurator;

class WareConfiguratorPanel : public QGroupBox {
    Q_OBJECT

public:
    WareConfiguratorPanel(const Settings &settings, QWidget *parent = nullptr);

    ~WareConfiguratorPanel() override;

public slots:
    void addWare(t_ware_id ware_id, bool is_secondary = false, unsigned amount = 0);

    void productionTargetUpdate();

signals:
    void shouldUpdate(t_x4_complex complex);

private:
    Ui::WareConfiguratorPanel *ui;
    std::unordered_map<t_ware_id, WareConfigurator *, std::hash<std::string> > ware_configurators;
    WareTargetContainer ware_target_container;
    t_target_list ware_targets;
    const Settings &settings_;
};

#endif // WARECONFIGURATORPANEL_HPP
