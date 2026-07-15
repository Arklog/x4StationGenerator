//
// Created by pierre on 7/17/25.
//

#ifndef WARECONFIGURATORPANEL_HPP
#define WARECONFIGURATORPANEL_HPP

#include <QFrame>

#include "ui_wareconfigurator.h"

#include "libcommon/data/Store.hpp"
#include "libcommon/stationbuilder/defines.hpp"
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
    using Settings            = common::stationbuilder::Settings;
    using Store               = common::data::Store;
    using t_ware_id           = common::types::Ware::ware_id;
    using t_x4_complex        = common::stationbuilder::t_x4_complex;
    using WareTargetContainer = common::utils::WareTargetContainer;

    WareConfiguratorPanel(const Settings &settings, const Store &store,
                          QWidget *       parent = nullptr);

    ~WareConfiguratorPanel() override;

public slots:
    void addWare(t_ware_id ware_id, bool is_secondary = false,
                 unsigned  amount                     = 0);

    void productionTargetUpdate();

signals:
    void shouldUpdate(t_x4_complex complex);

private:
    Ui::WareConfiguratorPanel *ui;
    std::unordered_map<t_ware_id, WareConfigurator *, std::hash<std::string> >
    ware_configurators;
    WareTargetContainer ware_target_container;
    const Settings &    settings_;
    const Store &       store_;
};

#endif // WARECONFIGURATORPANEL_HPP
