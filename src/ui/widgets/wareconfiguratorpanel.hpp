//
// Created by pierre on 7/17/25.
//

#ifndef WARECONFIGURATORPANEL_HPP
#define WARECONFIGURATORPANEL_HPP

#include <QFrame>

#include "ui_wareconfigurator.h"

#include "Data/Data.hpp"


QT_BEGIN_NAMESPACE
namespace Ui { class WareConfiguratorPanel; }
QT_END_NAMESPACE

class WareConfigurator;

class WareConfiguratorPanel : public QFrame {
Q_OBJECT

public:
    explicit WareConfiguratorPanel(QWidget *parent = nullptr);
    ~WareConfiguratorPanel() override;

public slots:
    void addWare(t_ware_id ware_id);

private:
    Ui::WareConfiguratorPanel *ui;
    std::unordered_map<t_ware_id, WareConfigurator *> ware_configurators;
};


#endif //WARECONFIGURATORPANEL_HPP
