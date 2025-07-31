//
// Created by pierre on 7/16/25.
//

#ifndef WARESELECTIONSECTION_H
#define WARESELECTIONSECTION_H

#include <QWidget>

#include "StationBuilder/defines.hpp"


QT_BEGIN_NAMESPACE
namespace Ui { class WareSelectionSection; }
QT_END_NAMESPACE

class WareSelectionSection : public QWidget {
Q_OBJECT

public:
    explicit WareSelectionSection(Settings &settings, QWidget *parent = nullptr);
    ~WareSelectionSection() override;

    const t_x4_complex& getComplex();

signals:
    void complexUpdated();

private:
    Ui::WareSelectionSection *ui;
    t_x4_complex complex_;
    Settings &settings_;
};


#endif //WareSelectionSection_H
