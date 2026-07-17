//
// Created by pierre on 7/21/25.
//

#ifndef DOCKANDPIERRCONFIGURATION_HPP
#define DOCKANDPIERRCONFIGURATION_HPP

#include <QWidget>

#include "libcommon/stationbuilder/defines.hpp"


struct Module;

QT_BEGIN_NAMESPACE

namespace Ui {
    class ModuleConfiguration;
}

QT_END_NAMESPACE

class ModuleConfiguration : public QWidget {
    Q_OBJECT

public:
    using Module = common::types::module::Module;

    explicit ModuleConfiguration(const Module *module, common::stationbuilder::ModuleTarget &target,
                                 QWidget *     parent = nullptr);

    ~ModuleConfiguration() override;

    common::stationbuilder::ModuleTarget getModuleTarget() const;

signals:
    void shouldRemove(const common::stationbuilder::ModuleTarget &target);

    void moduleTargetUpdated(const common::stationbuilder::ModuleTarget &target);

private:
    Ui::ModuleConfiguration *             ui;
    const Module *                        dock_or_pierr;
    common::stationbuilder::ModuleTarget &module_target_;
};


#endif //DOCKANDPIERRCONFIGURATION_HPP
