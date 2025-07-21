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
    class DockAndPierrSelectionPanel;
}

QT_END_NAMESPACE

class DockAndPierrSelectionPanel : public QFrame {
    Q_OBJECT

public:
    explicit DockAndPierrSelectionPanel(QWidget *parent = nullptr);

    ~DockAndPierrSelectionPanel() override;

signals:
    void moduleSelected(const TmpModule *module);

private:
    Ui::DockAndPierrSelectionPanel *ui;
};


#endif //DOCKANDPIERRSELECTIONPANEL_HPP
