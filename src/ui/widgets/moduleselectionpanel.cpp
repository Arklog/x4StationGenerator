//
// Created by pierre on 7/21/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DockAndPierrSelectionPanel.h" resolved


#include <QPushButton>

#include "moduleselectionpanel.hpp"
#include "ui_moduleselectionpanel.h"


ModuleSelectionPanel::ModuleSelectionPanel(const ModuleData::t_module_map &module_list,
                                           QWidget *parent) : QFrame(parent), ui(new Ui::ModuleSelectionPanel) {
    QFrame::setFrameShape(QFrame::StyledPanel);

    ui->setupUi(this);
    ui->layout->setAlignment(Qt::AlignTop);

    for (const auto &[module_id, module]: module_list) {
        auto module_button = new QPushButton(QString::fromStdString(module->name));
        this->layout()->addWidget(module_button);

        connect(module_button, &QPushButton::clicked, [this, module](bool clicked) -> void {
            emit moduleSelected(module);
        });
    }
}

ModuleSelectionPanel::~ModuleSelectionPanel() {
    delete ui;
}
