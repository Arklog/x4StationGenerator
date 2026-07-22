//
// Created by pierre on 7/22/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_OverviewSummary.h" resolved

#include "overviewsummary.hpp"

#include <QFormLayout>
#include <QGridLayout>

#include "ui_overviewsummary.h"
#include "stationbuilder/Complex.hpp"

namespace ui::section::summarysection::widgets {
    OverviewSummary::OverviewSummary(QWidget *parent) :
    QGroupBox("Overview", parent),
    ui(new Ui::OverviewSummary),
    widgets_(this) {
        ui->setupUi(this);

        auto layout            = new QGridLayout;
        auto station_name      = new QLabel("Station Name:", this);
        auto sun_factor        = new QLabel("Sun", this);
        auto station_cost      = new QLabel("Station Cost:", this);
        auto wares_price_total = new QLabel("Wares Price:", this);
        auto workforce_total   = new QLabel("Workforce:", this);
        auto container_storage = new QLabel("Container:", this);
        auto liquid_storage    = new QLabel("Liquid:", this);
        auto solid_storage     = new QLabel("Solid:", this);
        auto pier_number       = new QLabel("Pier Number:", this);
        auto dock_number       = new QLabel("Dock Number:", this);

        auto add_items = [&](QWidget *first, QWidget *second, int row, int col, int rowspan, int colspan) {
            auto form = new QFormLayout;
            form->addRow(first, second);
            layout->addLayout(form, row, col, rowspan, colspan);
        };

        add_items(station_name, widgets_.station_name, 0, 0, 1, 1);
        add_items(sun_factor, widgets_.sun_factor, 1, 0, 1, 1);
        add_items(station_cost, widgets_.station_cost, 2, 0, 1, 1);
        add_items(wares_price_total, widgets_.wares_price_total, 3, 0, 1, 1);
        add_items(workforce_total, widgets_.workforce_total, 4, 0, 1, 1);

        add_items(container_storage, widgets_.container_storage, 0, 1, 1, 1);
        add_items(liquid_storage, widgets_.liquid_storage, 1, 1, 1, 1);
        add_items(solid_storage, widgets_.solid_storage, 2, 1, 1, 1);
        add_items(pier_number, widgets_.pier_number, 3, 1, 1, 1);
        add_items(dock_number, widgets_.dock_number, 4, 1, 1, 1);

        this->setLayout(layout);
    }

    OverviewSummary::~OverviewSummary() {
        delete ui;
    }

    void OverviewSummary::update(const common::stationbuilder::Complex &complex, const common::data::Store &store) {
        int station_cost{};
        int dock_number{};
        int pier_number{};
        int container_storage{};
        int liquid_storage{};
        int solid_storage{};

        std::ranges::for_each(complex.complex, [&](const auto &module_id) {
            auto &module = store.modules.by_id.at(module_id)->module.get();

            station_cost += module.price.avg;

            if (module.module_type == common::types::module::ModuleType::storage) {
                auto storage = store.storages.by_id.at(module_id);
                int *v       = (storage->type == "container")
                             ? &container_storage
                             : (storage->type == "liquid")
                                   ? &liquid_storage
                                   : &solid_storage;
                *v += storage->capacity;
            } else if (module.module_type == common::types::module::ModuleType::dock) {
                ++dock_number;
            } else if (module.module_type == common::types::module::ModuleType::pier) {
                ++pier_number;
            }
        });

        int ware_price{};

        std::ranges::for_each(complex.wares.getTargets(), [&](const auto &ware_target) {
            auto ware  = store.wares.by_id.at(ware_target.ware_id);
            ware_price += ware->price.avg * ware_target.prodution;
        });

        widgets_.station_name->setText(QString::fromStdString(complex.name));
        widgets_.sun_factor->setText(QLocale().toString(complex.sun));
        widgets_.station_cost->setText(QLocale().toString(station_cost));
        widgets_.wares_price_total->setText(QLocale().toString(ware_price));
        widgets_.workforce_total->setText(QLocale().toString(complex.workforce));
        widgets_.dock_number->setText(QLocale().toString(dock_number));
        widgets_.pier_number->setText(QLocale().toString(pier_number));
        widgets_.container_storage->setText(QLocale().toString(container_storage));
        widgets_.liquid_storage->setText(QLocale().toString(liquid_storage));
        widgets_.solid_storage->setText(QLocale().toString(solid_storage));
    }

    OverviewSummary::Widgets::Widgets(QWidget *parent) :
    station_name(new QLabel(parent)),
    sun_factor(new QLabel(parent)),
    station_cost(new QLabel(parent)),
    wares_price_total(new QLabel(parent)),
    workforce_total(new QLabel(parent)),
    dock_number(new QLabel(parent)),
    pier_number(new QLabel(parent)),
    container_storage(new QLabel(parent)),
    liquid_storage(new QLabel(parent)),
    solid_storage(new QLabel(parent)) {
    }
} // ui::section::summarysection::widgets
