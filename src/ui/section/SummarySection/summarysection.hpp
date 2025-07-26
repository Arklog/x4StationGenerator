//
// Created by pierre on 7/25/25.
//

#ifndef SUMMARYSECTION_HPP
#define SUMMARYSECTION_HPP

#include <QWidget>

#include "StationBuilder/defines.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class SummarySection; }
QT_END_NAMESPACE

class SummarySection : public QWidget {
Q_OBJECT

public:
    explicit SummarySection(QWidget *parent = nullptr);
    ~SummarySection() override;

    void updateTargetList(const t_x4_complex &targets);

private:
    Ui::SummarySection *ui;
};


#endif //SUMMARYSECTION_HPP
