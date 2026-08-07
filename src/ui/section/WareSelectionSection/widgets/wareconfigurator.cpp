//
// Created by pierre on 7/17/25.
//

// You may need to build the project (run Qt uic code generator) to get
// "ui_WareConfigurator.h" resolved

#include "wareconfigurator.hpp"

#include <sys/stat.h>

#include "ui_wareconfigurator.h"

#include "data/WareModuleAndWorkforce.hpp"

#include "spdlog/spdlog.h"
#include "utils/WareTargetContainer.hpp"

WareConfigurator::WareConfigurator(t_ware_id ware_target_id, ui::utils::SharedState &state, const Store &store,
                                   QWidget * parent) :
QFrame(parent),
ui(new Ui::WareConfigurator),
ware_id{ware_target_id},
state_(state),
store_{store} {
    ui->setupUi(this);
    QFrame::setFrameShape(QFrame::StyledPanel);

    auto        settings    = state.settings();
    auto        ware_target = settings->ware_targets.getTarget(ware_target_id);
    const auto &ware        = store_.wares.by_id.at(ware_target_id);
    const auto &ware_name   = ware->name;

    decltype(store_.production.producing)::value_type::second_type possible_source_modules{};
    if (store_.production.producing.contains(ware_target_id))
        possible_source_modules = store_.production.producing.at(ware_target_id);

    ui->ware_label->setText(QString(ware_name.c_str()));
    ui->target_input->setValue(ware_target->production);

    // Create combobox of possible modules
    size_t i = 0;
    for (const auto &module: possible_source_modules) {
        auto module_id = module->module.get().id;
        auto text      = QString::fromStdString(module->module.get().name);
        ui->production_method_combo_box->addItem(text);

        if (module_id == ware_target->source_module)
            ui->production_method_combo_box->setCurrentIndex(i);
        ++i;
    }

    if (possible_source_modules.size() <= 1)
        ui->production_method_combo_box->setDisabled(true);

    // Is triggered when the ware amount required is changed
    auto trigger_update_target = [this](int value) -> void {
        spdlog::info("{} target value changed {}", this->ware_id, value);
        auto settings      = this->state_.settings();
        auto target        = settings->ware_targets.getTarget(this->ware_id);
        target->production = value;
        // this->shouldUpdate();
    };

    // Is triggered when the source module is changed
    auto trigger_update_source_module = [this](const QString &new_id) -> void {
        const auto &production_method = this->store_.modules.by_name.at(new_id.toStdString())->module.get().id;
        spdlog::info("{} production method changed {}",
                     this->ware_id, production_method);
        auto settings              = this->state_.settings();
        auto ware_target           = settings->ware_targets.getTarget(this->ware_id);
        ware_target->source_module = production_method;
        // this->shouldUpdate();
    };

    connect(ui->remove_button, &QPushButton::clicked,
            [this, ware_target_id](bool clicked) {
                spdlog::info("Removing ware {}", ware_target_id);
                emit this->shouldRemove(ware_target_id);
            });
    connect(ui->production_method_combo_box, &QComboBox::currentTextChanged,
            trigger_update_source_module);
    connect(ui->target_input, &QSpinBox::valueChanged, trigger_update_target);

    // in case of secondary ware, we gray out the widget and disable input
    // except for the source module selection
    if (!ware_target->is_secondary)
        return;
    auto palete = this->palette();
    palete.setColor(QPalette::Window, QColor(200, 200, 200));
    this->setAutoFillBackground(true);
    this->setPalette(palete);

    ui->target_input->setReadOnly(true);
    ui->target_input->setDisabled(true);
    ui->remove_button->setDisabled(true);
}

WareConfigurator::~WareConfigurator() { delete ui; }

const common::utils::WareTarget *WareConfigurator::getWareTarget() const {
    auto settings    = this->state_.settings();
    auto ware_target = settings->ware_targets.getTarget(this->ware_id);
    return ware_target;
}
