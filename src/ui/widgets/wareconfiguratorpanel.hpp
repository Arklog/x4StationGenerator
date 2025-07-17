//
// Created by pierre on 7/17/25.
//

#ifndef WARECONFIGURATORPANEL_HPP
#define WARECONFIGURATORPANEL_HPP

#include <QWidget>

#include "Data/Data.hpp"


QT_BEGIN_NAMESPACE
namespace Ui { class WareConfiguratorPanel; }
QT_END_NAMESPACE

class WareConfiguratorPanel : public QWidget {
Q_OBJECT

public:
    explicit WareConfiguratorPanel(QWidget *parent = nullptr);
    ~WareConfiguratorPanel() override;

public slots:
    void addWare(t_ware_id ware_id);

private:
    Ui::WareConfiguratorPanel *ui;
};


#endif //WARECONFIGURATORPANEL_HPP
