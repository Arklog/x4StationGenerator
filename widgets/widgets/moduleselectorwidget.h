#ifndef MODULESELECTOR_H
#define MODULESELECTOR_H

#include <QWidget>
#include "modules.hpp"

namespace Ui {
    class ModuleSelectorWidget;
}

class ModuleSelectorWidget : public QWidget {
Q_OBJECT

public:
    explicit ModuleSelectorWidget(QWidget *parent = nullptr);

    ~ModuleSelectorWidget();

    const Module &getModule() const;

    int getModuleNumber() const;

    std::map<RESSOURCE, int> getProduction() const;

    std::map<RESSOURCE, int> getConsumption() const;

signals:

    void moduleNumberChanged(const ModuleSelectorWidget &moduleSelector);

private:
    Ui::ModuleSelectorWidget *ui;
};

#endif // MODULESELECTOR_H
