//
// Created by pierre on 7/21/25.
//

#ifndef DOCKANDPIERRSELECTIONPANEL_HPP
#define DOCKANDPIERRSELECTIONPANEL_HPP

#include <QFrame>
#include <QWidget>

#include "Data/WareModuleAndWorkforce.hpp"

#include "StationBuilder/defines.hpp"

struct Module;

QT_BEGIN_NAMESPACE

namespace Ui {
    class ModuleSelectionPanel;
}

QT_END_NAMESPACE

class ModuleSelectionPanel : public QFrame {
    Q_OBJECT

public:
    explicit ModuleSelectionPanel(const t_module_list& module_list, QWidget *parent = nullptr);

    ~ModuleSelectionPanel() override;

signals:
    void moduleSelected(const Module *module);

private:
    Ui::ModuleSelectionPanel *ui;
};


#endif //DOCKANDPIERRSELECTIONPANEL_HPP
