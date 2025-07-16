//
// Created by pierre on 2/15/25.
//

#ifndef X4STATIONGENERATOR_MODULESSECTION_HPP
#define X4STATIONGENERATOR_MODULESSECTION_HPP

#include <QWidget>
#include "defines.hpp"
#include "ui/widgets/moduleselectorwidget.h"
#include "BuildSettings.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class ModulesSection; }
QT_END_NAMESPACE

class ModulesSection : public QWidget {
Q_OBJECT

public:
    ModulesSection(BuildSettings &settings, QWidget *parent = nullptr);

    ~ModulesSection() override;

    const t_modules &getModules() const;

public slots:

    void addModule();

    void updateModules(const ModuleSelectorWidget &widget);

    void updateStationName(const QString &text);

signals:

    void moduleUpdated(const t_modules &modules);

private:
    Ui::ModulesSection *ui;
    t_modules          _modules;
    BuildSettings      &_settings;
};


#endif //X4STATIONGENERATOR_MODULESSECTION_HPP
