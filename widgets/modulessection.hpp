//
// Created by pierre on 2/15/25.
//

#ifndef X4STATIONGENERATOR_MODULESSECTION_HPP
#define X4STATIONGENERATOR_MODULESSECTION_HPP

#include <QWidget>
#include "defines.hpp"
#include "moduleselectorwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ModulesSection; }
QT_END_NAMESPACE

class ModulesSection : public QWidget {
Q_OBJECT

public:
    explicit ModulesSection(QWidget *parent = nullptr);

    ~ModulesSection() override;

public slots:

    void addModule();

    void updateModules(const ModuleSelectorWidget &widget);

signals:

    void moduleUpdated(const t_modules &modules);

private:
    Ui::ModulesSection *ui;
    t_modules          _modules;
};


#endif //X4STATIONGENERATOR_MODULESSECTION_HPP
