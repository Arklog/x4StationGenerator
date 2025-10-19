//
// Created by pierre on 7/17/25.
//

#ifndef WARESSELECTOR_HPP
#define WARESSELECTOR_HPP

#include <QWidget>

#include "Data/RawData.hpp"

QT_BEGIN_NAMESPACE

namespace Ui {
    class WaresSelector;
}

QT_END_NAMESPACE

class WaresSelector : public QWidget {
    Q_OBJECT

public:
    explicit WaresSelector(QWidget *parent = nullptr);

    ~WaresSelector() override;

signals:
    void wareSelected(t_ware_id);

private:
    Ui::WaresSelector *ui;
    std::unordered_map<t_ware_group_id, QWidget *> category_tabs;
};


#endif //WARESSELECTOR_HPP
