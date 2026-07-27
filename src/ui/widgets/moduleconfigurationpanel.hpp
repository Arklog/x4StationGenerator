//
// Created by pierre on 7/21/25.
//

#ifndef DOCKANDPIERRCONFIGURATIONPANEL_HPP
#define DOCKANDPIERRCONFIGURATIONPANEL_HPP

#include <QFrame>
#include <QWidget>

namespace ui::utils {
    class SharedState;
}

namespace common::stationbuilder {
    struct Settings;
}

QT_BEGIN_NAMESPACE

namespace Ui {
    class ModuleConfigurationPanel;
}

QT_END_NAMESPACE

class ModuleConfigurationPanel : public QFrame {
    Q_OBJECT

public:
    using Module             = common::types::module::Module;
    using module_list_target = common::stationbuilder::t_module_target_list common::stationbuilder::Settings::*;

    explicit ModuleConfigurationPanel(ui::utils::SharedState &state, module_list_target target,
                                      QWidget *               parent = nullptr);

    ~ModuleConfigurationPanel() override;

    common::stationbuilder::t_module_target_list getModuleTargets() const;

public slots:
    void addModule(const Module *module);

private:
    Ui::ModuleConfigurationPanel *ui;
    ui::utils::SharedState &      state_;
    module_list_target            member_;
};


#endif //DOCKANDPIERRCONFIGURATIONPANEL_HPP
