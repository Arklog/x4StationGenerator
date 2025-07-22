//
// Created by pierre on 7/21/25.
//

#ifndef DOCKANDPIERRSELECTIONPANEL_HPP
#define DOCKANDPIERRSELECTIONPANEL_HPP

#include <QFrame>
#include <QWidget>

struct TmpModule;

QT_BEGIN_NAMESPACE

namespace Ui {
    class ModuleSelectionPanel;
}

QT_END_NAMESPACE

class ModuleSelectionPanel : public QFrame {
    Q_OBJECT

public:
    explicit ModuleSelectionPanel(QWidget *parent = nullptr);

    ~ModuleSelectionPanel() override;

signals:
    void moduleSelected(const TmpModule *module);

private:
    Ui::ModuleSelectionPanel *ui;
};


#endif //DOCKANDPIERRSELECTIONPANEL_HPP
