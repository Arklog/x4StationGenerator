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

    [[nodiscard]] const Module &getModule() const;

    [[nodiscard]] int getModuleNumber() const;

    [[nodiscard]] std::map<RESSOURCE, int> getProduction() const;

    [[nodiscard]] std::map<RESSOURCE, int> getConsumption() const;

    [[nodiscard]] const Module *getFormerType() const;

signals:

    void moduleUpdated(const ModuleSelectorWidget &moduleSelector);

private:
    Ui::ModuleSelectorWidget *ui;
    const Module             *_former_type;
};

#endif // MODULESELECTOR_H
