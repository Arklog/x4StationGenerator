//
// Created by pierre on 7/16/25.
//

#ifndef WARESELECTIONSECTION_H
#define WARESELECTIONSECTION_H

#include "data/Store.hpp"

#include <QWidget>

#include "stationbuilder/defines.hpp"

QT_BEGIN_NAMESPACE

namespace Ui {
    class WareSelectionSection;
}

QT_END_NAMESPACE

class WareSelectionSection : public QWidget {
    Q_OBJECT

public:
    using Store        = common::data::Store;
    using t_x4_complex = common::stationbuilder::t_x4_complex;
    using Settings     = common::stationbuilder::Settings;
    using t_ware_id    = common::types::Ware::ware_id;

    explicit WareSelectionSection(Settings &settings, const Store &store,
                                  QWidget * parent = nullptr);

    ~WareSelectionSection() override;

    const t_x4_complex &getComplex();

signals:
    void complexUpdated();

private:
    Ui::WareSelectionSection *ui;
    t_x4_complex              complex_;
    Settings &                settings_;
    Store                     store_;
};

#endif // WareSelectionSection_H
