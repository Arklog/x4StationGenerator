//
// Created by pierre on 7/21/25.
//

#ifndef DOCKANDPIERRSECTION_HPP
#define DOCKANDPIERRSECTION_HPP

#include <QWidget>

#include "StationBuilder/defines.hpp"


class DockAndPierrConfigurationPanel;
QT_BEGIN_NAMESPACE
namespace Ui { class DockAndPierrSection; }
QT_END_NAMESPACE

class DockAndPierrSection : public QWidget {
Q_OBJECT

public:
    explicit DockAndPierrSection(QWidget *parent = nullptr);
    ~DockAndPierrSection() override;

    t_module_target_list getModuleTargetList() const;

private:
    Ui::DockAndPierrSection *ui;
    DockAndPierrConfigurationPanel *dock_and_pierr_configuration_panel;
};


#endif //DOCKANDPIERRSECTION_HPP
