//
// Created by pierre on 7/16/25.
//

#ifndef WARESELECTIONSECTION_H
#define WARESELECTIONSECTION_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class wareselectionsection; }
QT_END_NAMESPACE

class wareselectionsection : public QWidget {
Q_OBJECT

public:
    explicit wareselectionsection(QWidget *parent = nullptr);
    ~wareselectionsection() override;

private:
    Ui::wareselectionsection *ui;
};


#endif //WARESELECTIONSECTION_H
