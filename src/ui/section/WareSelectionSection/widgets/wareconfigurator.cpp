//
// Created by pierre on 7/17/25.
//

// You may need to build the project (run Qt uic code generator) to get
// "ui_WareConfigurator.h" resolved

#include "wareconfigurator.hpp"
#include "ui_wareconfigurator.h"

#include "Data/Data.hpp"

#include "Data/WareModuleAndWorkforce.hpp"

#include "spdlog/spdlog.h"

WareConfigurator::WareConfigurator(const t_ware_id &ware_id, QWidget *parent)
    : QFrame(parent), ui(new Ui::WareConfigurator),
      ware_target{.ware_id = ware_id} {
  ui->setupUi(this);
  QFrame::setFrameShape(QFrame::StyledPanel);

  const auto &ware = getWares().at(ware_id);
  const auto &ware_name = ware->name;
  const auto &possible_source_modules = getModules(ware_id);

  ui->ware_label->setText(QString(ware_name.c_str()));

  this->ware_target.source_module = possible_source_modules.begin()->first;

  // Create combobox of possible modules
  for (const auto &[module_id, module] : possible_source_modules) {
    auto text = QString::fromStdString(module->name);
    ui->production_method_combo_box->addItem(text);
  }

  this->ware_target.prodution = ui->target_input->value();
  this->ware_target.source_module = getModuleIdFromName(
      ui->production_method_combo_box->currentText().toStdString());

  // Is triggered when the ware amount required is changed
  auto trigger_update_target = [this](int value) -> void {
    spdlog::info("{} target value changed {}", this->ware_target.ware_id,
                 value);
    this->ware_target.prodution = value;
    this->shouldUpdate();
  };

  // Is triggered when the source module is changed
  auto trigger_update_source_module =
      [this](const QString &new_id) -> void {
    spdlog::info("{} production method changed {}", this->ware_target.ware_id,
                 new_id.toStdString());
    const auto &production_method =
        getModuleIdFromName(new_id.toStdString());
    this->ware_target.source_module = new_id.toStdString();
    this->shouldUpdate();
  };

  connect(ui->remove_button, &QPushButton::clicked,
          [this, ware_id](bool clicked) {
            spdlog::info("Removing ware {}", ware_id);
            this->shouldRemove(this->ware_target.ware_id);
          });
  connect(ui->production_method_combo_box, &QComboBox::currentTextChanged,
          trigger_update_source_module);
  connect(ui->target_input, &QSpinBox::valueChanged, trigger_update_target);
}

WareConfigurator::~WareConfigurator() { delete ui; }

const WareTarget *WareConfigurator::getWareTarget() const {
  return &(this->ware_target);
}
