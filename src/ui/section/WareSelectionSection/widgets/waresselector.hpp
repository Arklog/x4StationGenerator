//
// Created by pierre on 7/17/25.
//

#ifndef WARESSELECTOR_HPP
#define WARESSELECTOR_HPP

#include <QWidget>

#include "data/Store.hpp"

QT_BEGIN_NAMESPACE

namespace Ui {
    class WaresSelector;
}

QT_END_NAMESPACE

class WaresSelector : public QWidget {
    Q_OBJECT

public:
    explicit WaresSelector(const common::data::Store &store, QWidget *parent = nullptr);

    ~WaresSelector() override;

    signals:


    void wareSelected(common::types::Ware::ware_id);

private:
    Ui::WaresSelector *                                               ui;
    std::unordered_map<common::types::Ware::ware_group_id, QWidget *> category_tabs;

    void populateGroup(QWidget *widget, const std::vector<common::types::Ware *> &wares);
};

#endif // WARESSELECTOR_HPP
