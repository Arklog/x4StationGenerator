//
// Created by pierre on 2/15/25.
//

#ifndef X4STATIONGENERATOR_RESSOURCEPRODUCEDSECTION_HPP
#define X4STATIONGENERATOR_RESSOURCEPRODUCEDSECTION_HPP

#include <QWidget>
#include "utils.hpp"
#include "defines.hpp"
#include "modules.hpp"


QT_BEGIN_NAMESPACE
namespace Ui { class RessourceProducedSection; }
QT_END_NAMESPACE

class RessourceProducedSection : public QWidget {
Q_OBJECT

public:
    explicit RessourceProducedSection(QWidget *parent = nullptr);

    ~RessourceProducedSection() override;

public slots:

    void modulesUpdated(const t_modules &modules);

private:
    Ui::RessourceProducedSection *ui;
};


#endif //X4STATIONGENERATOR_RESSOURCEPRODUCEDSECTION_HPP
