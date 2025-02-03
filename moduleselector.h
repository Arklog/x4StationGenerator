#ifndef MODULESELECTOR_H
#define MODULESELECTOR_H

#include <QWidget>
#include "modules.hpp"

namespace Ui {
    class ModuleSelector;
}

class ModuleSelector : public QWidget {
Q_OBJECT

public:
    explicit ModuleSelector(QWidget *parent = nullptr);

    ~ModuleSelector();

    const Module &getModule() const;
    int getModuleNumber() const;

    std::map<RESSOURCES, int> getProduction() const;
    std::map<RESSOURCES, int> getConsumption() const;
signals:

    void moduleNumberChanged();

private:
    Ui::ModuleSelector *ui;
};

#endif // MODULESELECTOR_H
