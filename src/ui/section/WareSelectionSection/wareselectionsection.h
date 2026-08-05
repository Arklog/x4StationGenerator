//
// Created by pierre on 7/16/25.
//

#ifndef WARESELECTIONSECTION_H
#define WARESELECTIONSECTION_H

#include "data/Store.hpp"

#include <QWidget>

#include "stationbuilder/Complex.hpp"
#include "stationbuilder/defines.hpp"

namespace ui::utils {
    class SharedState;
}

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
    using t_ware_id    = common::types::Ware::ware_id;

    explicit WareSelectionSection(ui::utils::SharedState &settings, const Store &store,
                                  QWidget *               parent = nullptr);

    ~WareSelectionSection() override;

signals:
    void complexUpdated();

private:
    Ui::WareSelectionSection *ui;
    ui::utils::SharedState &  state_;
    Store                     store_;
};

#endif // WareSelectionSection_H
