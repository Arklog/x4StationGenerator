//
// Created by pierre on 2/9/25.
//

#include "stationsizewidget.h"
#include "ui_stationsizewidget.h"

StationSizeWidget::StationSizeWidget(QWidget *parent) : QWidget(parent), ui(new Ui::StationSizeWidget) {
    ui->setupUi(this);

    connect(ui->x_plus, &QSlider::valueChanged, this, &StationSizeWidget::updateLabels);
    connect(ui->x_minus, &QSlider::valueChanged, this, &StationSizeWidget::updateLabels);
    connect(ui->y_plus, &QSlider::valueChanged, this, &StationSizeWidget::updateLabels);
    connect(ui->y_minus, &QSlider::valueChanged, this, &StationSizeWidget::updateLabels);
    connect(ui->z_plus, &QSlider::valueChanged, this, &StationSizeWidget::updateLabels);
    connect(ui->z_minus, &QSlider::valueChanged, this, &StationSizeWidget::updateLabels);

    this->updateLabels();
}

void StationSizeWidget::updateLabels() {
    ui->x_plus->setMaximum(20 - ui->x_minus->value());
    ui->x_minus->setMaximum(20 - ui->x_plus->value());
    ui->y_plus->setMaximum(20 - ui->y_minus->value());
    ui->y_minus->setMaximum(20 - ui->y_plus->value());
    ui->z_plus->setMaximum(20 - ui->z_minus->value());
    ui->z_minus->setMaximum(20 - ui->z_plus->value());
    ui->x_plus_label->setText(QString("-x: %1").arg(ui->x_plus->value()));
    ui->x_minus_label->setText(QString("+x: %1").arg(ui->x_minus->value()));
    ui->y_plus_label->setText(QString("-y: %1").arg(ui->y_plus->value()));
    ui->y_minus_label->setText(QString("+y: %1").arg(ui->y_minus->value()));
    ui->z_plus_label->setText(QString("-z: %1").arg(ui->z_plus->value()));
    ui->z_minus_label->setText(QString("+z: %1").arg(ui->z_minus->value()));
}

StationSize StationSizeWidget::getSize() {
    return StationSize{
            ui->x_plus->value(),
            ui->x_minus->value(),
            ui->y_plus->value(),
            ui->y_minus->value(),
            ui->z_plus->value(),
            ui->z_minus->value()
    };
}
