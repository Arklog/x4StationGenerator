//
// Created by pierre on 7/21/25.
//

#ifndef DOCKANDPIERRCONFIGURATIONPANEL_HPP
#define DOCKANDPIERRCONFIGURATIONPANEL_HPP

#include <QFrame>
#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
    class ModuleConfigurationPanel;
}

QT_END_NAMESPACE

class ModuleConfigurationPanel : public QFrame {
    Q_OBJECT

public:
    using Module = common::types::module::Module;

    explicit ModuleConfigurationPanel(QWidget *parent = nullptr);

    ~ModuleConfigurationPanel() override;

    common::stationbuilder::t_module_target_list getModuleTargets() const;

public slots:
    void addModule(const Module *dock_or_pierr);

signals:
    void targetListUpdated();

private:
    Ui::ModuleConfigurationPanel *                    ui;
    std::vector<common::stationbuilder::ModuleTarget> module_targets_{};
};


#endif //DOCKANDPIERRCONFIGURATIONPANEL_HPP
