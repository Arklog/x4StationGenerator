//
// Created by pierre on 2/15/25.
//

#ifndef X4STATIONGENERATOR_BUILDSUMMARYSECTION_HPP
#define X4STATIONGENERATOR_BUILDSUMMARYSECTION_HPP

#include <QWidget>
#include "defines.hpp"
#include "utils.hpp"
#include "StationBuilder/StationBuilder.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class BuildSummarySection; }
QT_END_NAMESPACE

class BuildSummarySection : public QWidget {
Q_OBJECT

public:
    explicit BuildSummarySection(QWidget *parent = nullptr);

    ~BuildSummarySection() override;

public slots:

    void modulesUpdated(const StationBuilder &builder);

signals:

    void exportStation();

private:
    Ui::BuildSummarySection *ui;
};


#endif //X4STATIONGENERATOR_BUILDSUMMARYSECTION_HPP
