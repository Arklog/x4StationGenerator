//
// Created by pierre on 7/21/25.
//

#ifndef DOCKANDPIERRCONFIGURATIONPANEL_HPP
#define DOCKANDPIERRCONFIGURATIONPANEL_HPP

#include <QFrame>
#include <QWidget>

#include "Data/Data.hpp"

#include "StationBuilder/defines.hpp"


QT_BEGIN_NAMESPACE

namespace Ui {
    class ModuleConfigurationPanel;
}

QT_END_NAMESPACE

class ModuleConfigurationPanel : public QFrame {
    Q_OBJECT

public:
    explicit ModuleConfigurationPanel(QWidget *parent = nullptr);

    ~ModuleConfigurationPanel() override;

    t_module_target_list getModuleTargets() const;

public slots:
    void addModule(const Module *dock_or_pierr);

signals:
    void targetListUpdated(t_module_target_list targets);

private:
    Ui::ModuleConfigurationPanel *ui;
    std::vector<ModuleTarget> module_targets_{};
};


#endif //DOCKANDPIERRCONFIGURATIONPANEL_HPP
