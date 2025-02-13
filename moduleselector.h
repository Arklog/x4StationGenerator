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

    std::map<RESSOURCE, int> getProduction() const;

    std::map<RESSOURCE, int> getConsumption() const;

signals:

    void moduleNumberChanged();

private:
    Ui::ModuleSelector *ui;
};

#endif // MODULESELECTOR_H
