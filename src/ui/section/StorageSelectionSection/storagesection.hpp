//
// Created by pierre on 7/22/25.
//

#ifndef STORAGESELECTIONSECTION_HPP
#define STORAGESELECTIONSECTION_HPP

#include <QWidget>

#include "StationBuilder/defines.hpp"


class ModuleSelectionPanel;
class ModuleConfigurationPanel;

QT_BEGIN_NAMESPACE
namespace Ui { class StorageSection; }
QT_END_NAMESPACE

class StorageSection : public QWidget {
Q_OBJECT

public:
    explicit StorageSection(QWidget *parent = nullptr);
    ~StorageSection() override;

    t_module_target_list getModuleTargetList() const;

signals:
    void storageUpdated();

private:
    Ui::StorageSection *ui;

    ModuleSelectionPanel *storage_selection_panel;
    ModuleConfigurationPanel *storage_configuration_panel;
};


#endif //STORAGESELECTIONSECTION_HPP
