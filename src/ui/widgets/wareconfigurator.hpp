//
// Created by pierre on 7/17/25.
//

#ifndef WARECONFIGURATOR_HPP
#define WARECONFIGURATOR_HPP

#include <QWidget>

#include "Data/Data.hpp"


QT_BEGIN_NAMESPACE

namespace Ui {
    class WareConfigurator;
}

QT_END_NAMESPACE

class WareConfigurator : public QWidget {
    Q_OBJECT

public:
    explicit WareConfigurator(t_ware_id ware_id, QWidget *parent = nullptr);

    ~WareConfigurator() override;

private:
    Ui::WareConfigurator *ui;
    t_ware_id ware_id;
};


#endif //WARECONFIGURATOR_HPP
