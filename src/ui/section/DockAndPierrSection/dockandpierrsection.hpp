//
// Created by pierre on 7/21/25.
//

#ifndef DOCKANDPIERRSECTION_HPP
#define DOCKANDPIERRSECTION_HPP

#include "libcommon/data/Store.hpp"

#include <QWidget>

#include "libcommon/stationbuilder/defines.hpp"


class ModuleConfigurationPanel;
QT_BEGIN_NAMESPACE
namespace Ui {
    class DockAndPierrSection;
}

QT_END_NAMESPACE

class DockAndPierrSection : public QWidget {
    Q_OBJECT

public:
    using Store                = common::data::Store;
    using t_module_target_list = common::stationbuilder::t_module_target_list;

    explicit DockAndPierrSection(const Store &store, QWidget *parent = nullptr);

    ~DockAndPierrSection() override;

    t_module_target_list getModuleTargetList() const;

signals:
    void dockAndPierrUpdated();

private:
    Ui::DockAndPierrSection *  ui;
    ModuleConfigurationPanel * dock_and_pierr_configuration_panel;
    const common::data::Store &store_;
};


#endif //DOCKANDPIERRSECTION_HPP
