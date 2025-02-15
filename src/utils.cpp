//
// Created by pierre on 2/15/25.
//
#include "utils.hpp"

void clearLayout(QLayout *layout)
{
    while (!layout->isEmpty()) {
        QWidget *widget = layout->itemAt(0)->widget();
        if (!widget)
            break;
        layout->removeWidget(widget);
        delete widget;
    }
}