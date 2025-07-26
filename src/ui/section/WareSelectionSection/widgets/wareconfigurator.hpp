//
// Created by pierre on 7/17/25.
//

#ifndef WARECONFIGURATOR_HPP
#define WARECONFIGURATOR_HPP

#include <QFrame>
#include <QWidget>

#include "Data/Data.hpp"

#include "StationBuilder/defines.hpp"


QT_BEGIN_NAMESPACE

namespace Ui {
    class WareConfigurator;
}

QT_END_NAMESPACE

class WareConfigurator : public QFrame {
    Q_OBJECT

public:
    explicit WareConfigurator(const t_ware_id& ware_id, QWidget *parent = nullptr);

    ~WareConfigurator() override;

    const WareTarget* getWareTarget() const;

signals:
    void shouldRemove(t_ware_id ware_id);
    void shouldUpdate();

private:
    Ui::WareConfigurator *ui;
    WareTarget ware_target;
};


#endif //WARECONFIGURATOR_HPP
