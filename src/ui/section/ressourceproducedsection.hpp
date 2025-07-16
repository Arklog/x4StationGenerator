//
// Created by pierre on 2/15/25.
//

#ifndef X4STATIONGENERATOR_RESSOURCEPRODUCEDSECTION_HPP
#define X4STATIONGENERATOR_RESSOURCEPRODUCEDSECTION_HPP

#include <QWidget>
#include "utils.hpp"
#include "defines.hpp"
#include "modules.hpp"
#include "StationBuilder/StationBuilder.hpp"
#include "BuildSettings.hpp"


QT_BEGIN_NAMESPACE
namespace Ui { class RessourceProducedSection; }
QT_END_NAMESPACE

class RessourceProducedSection : public QWidget {
Q_OBJECT

public:
    RessourceProducedSection(BuildSettings &settings, QWidget *parent = nullptr);

    ~RessourceProducedSection() override;

public slots:

    void modulesUpdated(const StationBuilder &builder);

private:
    Ui::RessourceProducedSection *ui;
    BuildSettings                &_settings;
};


#endif //X4STATIONGENERATOR_RESSOURCEPRODUCEDSECTION_HPP
