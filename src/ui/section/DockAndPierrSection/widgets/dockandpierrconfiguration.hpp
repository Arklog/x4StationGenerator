//
// Created by pierre on 7/21/25.
//

#ifndef DOCKANDPIERRCONFIGURATION_HPP
#define DOCKANDPIERRCONFIGURATION_HPP

#include <QWidget>

#include "StationBuilder/defines.hpp"


struct TmpModule;

QT_BEGIN_NAMESPACE

namespace Ui {
    class DockAndPierrConfiguration;
}

QT_END_NAMESPACE

class DockAndPierrConfiguration : public QWidget {
    Q_OBJECT

public:
    explicit DockAndPierrConfiguration(const TmpModule *dock_or_pierr, QWidget *parent = nullptr);

    ~DockAndPierrConfiguration() override;

    ModuleTarget getModuleTarget() const;

signals:
    void shouldRemove();

private:
    Ui::DockAndPierrConfiguration *ui;
    const TmpModule *dock_or_pierr;
};


#endif //DOCKANDPIERRCONFIGURATION_HPP
