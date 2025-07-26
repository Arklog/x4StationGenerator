//
// Created by pierre on 7/21/25.
//

#ifndef DOCKANDPIERRCONFIGURATION_HPP
#define DOCKANDPIERRCONFIGURATION_HPP

#include <QWidget>

#include "StationBuilder/defines.hpp"


struct Module;

QT_BEGIN_NAMESPACE

namespace Ui {
    class ModuleConfiguration;
}

QT_END_NAMESPACE

class ModuleConfiguration : public QWidget {
    Q_OBJECT

public:
    explicit ModuleConfiguration(const Module *module, ModuleTarget &target, QWidget *parent = nullptr);

    ~ModuleConfiguration() override;

    ModuleTarget getModuleTarget() const;

signals:
    void shouldRemove(const ModuleTarget &target);
    void moduleTargetUpdated(const ModuleTarget &target);

private:
    Ui::ModuleConfiguration *ui;
    const Module *dock_or_pierr;
    ModuleTarget &module_target_;
};


#endif //DOCKANDPIERRCONFIGURATION_HPP
