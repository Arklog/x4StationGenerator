//
// Created by pierre on 7/16/25.
//

#ifndef WARESELECTIONSECTION_H
#define WARESELECTIONSECTION_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class WareSelectionSection; }
QT_END_NAMESPACE

class WareSelectionSection : public QWidget {
Q_OBJECT

public:
    explicit WareSelectionSection(QWidget *parent = nullptr);
    ~WareSelectionSection() override;

private:
    Ui::WareSelectionSection *ui;
};


#endif //WareSelectionSection_H
