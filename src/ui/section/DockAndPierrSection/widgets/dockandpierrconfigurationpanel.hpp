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
    class DockAndPierrConfigurationPanel;
}

QT_END_NAMESPACE

class DockAndPierrConfigurationPanel : public QFrame {
    Q_OBJECT

public:
    explicit DockAndPierrConfigurationPanel(QWidget *parent = nullptr);

    ~DockAndPierrConfigurationPanel() override;

    t_module_target_list getDocksAndPierr() const;

public slots:
    void addDockOrPierr(const TmpModule *dock_or_pierr);

private:
    Ui::DockAndPierrConfigurationPanel *ui;
};


#endif //DOCKANDPIERRCONFIGURATIONPANEL_HPP
