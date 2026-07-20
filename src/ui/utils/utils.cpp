//
// Created by pierre on 7/21/26.
//

#include "utils.hpp"

#include <QLayout>
#include <QWidget>

void clearLayout(QLayout *layout) {
    while (auto item = layout->takeAt(0)) {
        if (auto widget = item->widget()) {
            widget->deleteLater();
        }
    }
}

void clearLayoutNoDelete(QLayout *layout) {
    while (layout->count()) {
        layout->takeAt(0);
    }
}
