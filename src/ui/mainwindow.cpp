#include "mainwindow.h"
#include <QFileDialog>
#include <QTextStream>
#include <QErrorMessage>

#include "utils/modules.hpp"

#include "ui_mainwindow.h"
#include "common/types/StationSaveFile.hpp"

#include "section/DockAndPierrSection/dockandpierrsection.hpp"
#include "section/SettingsSection/settingssection.hpp"
#include "section/StorageSelectionSection/storagesection.hpp"
#include "section/SummarySection/summarysection.hpp"
#include "section/WareSelectionSection/wareselectionsection.h"

#include <spdlog/spdlog.h>
#include <rfl/json.hpp>

MainWindow::MainWindow(const Store &store, QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow),
error_message_(new QErrorMessage{this}),
store_(store),
state{store} {
    ui->setupUi(this);

    auto ware_selection_section = new WareSelectionSection(state, store_, this);
    auto dock_and_pierr_section = new DockAndPierrSection(state, store, this);
    auto storage_section        = new StorageSection(state, store, this);
    auto settings_section       = new SettingsSection(state, this);
    auto summary_section        = new SummarySection(store, this);

    ui->ware_selection_tab_layout->addWidget(ware_selection_section);
    ui->dock_and_pierr_tab_layout->addWidget(dock_and_pierr_section);
    ui->storage_tab_layout->addWidget(storage_section);
    ui->summary_tab_layout->addWidget(summary_section);
    ui->settings_tab_layout->addWidget(settings_section);

    this->ware_selection_section_ = ware_selection_section;
    this->dock_and_pierr_section_ = dock_and_pierr_section;
    this->storage_section_        = storage_section;
    this->summary_section_        = summary_section;
    this->settings_section_       = settings_section;

    connect(&this->state, &ui::utils::SharedState::complexChanged, this, &MainWindow::complexUpdated);

    connect(ui->action_export, &QAction::triggered, this, &MainWindow::exportPlan);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::savePlan);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openPlan);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::exportPlan() {
    spdlog::info("Exporting plan");

    auto complex  = state.complex();
    auto settings = state.settings();

    auto filename = complex->name;
    std::ranges::transform(complex->name, filename.begin(), [](auto c) {
        if (std::isspace(c))
            return '_';
        return static_cast<char>(std::tolower(c));
    });

#ifdef WIN32
    auto home   = getenv("USERPROFILE");
    auto OUTDIR = fmt::format("{}/Documents/Egosoft/X4/1234567890", home);
#else
    auto home   = getenv("HOME");
    auto OUTDIR = fmt::format("{}/.config/EgoSoft/X4/371399543/constructionplan", home);
#endif

    spdlog::info(std::filesystem::canonical(OUTDIR).string());
    QFileDialog dialog(this, "Export plan",
                       QString::fromStdString(fmt::format("{}/{}.xml", OUTDIR, filename)));
    dialog.setFileMode(QFileDialog::FileMode::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptMode::AcceptSave);

    if (dialog.exec()) {
        auto selected_file = dialog.selectedFiles().first();
        spdlog::info("selected file: {0}", selected_file.toStdString());

        QFile file(selected_file);

        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            auto        data = genModulePlan(complex->complex, store_, settings);

            file.write(data.c_str());
        }
    }
}

void MainWindow::openPlan() {
    using StationSaveFile = common::types::StationSaveFile;

    try {
        QFileDialog dialog(this, "Open file");
        dialog.setFileMode(QFileDialog::FileMode::ExistingFile);

        if (dialog.exec()) {
            auto selected_file = dialog.selectedFiles().first();
            auto v             = rfl::json::load<StationSaveFile>(selected_file.toStdString());

            if (!v.has_value())
                throw std::runtime_error("Could not read file: " + v.error().what());

            auto settings = state.settings();
            settings      = Settings(std::move(v.value()), store_);
            emit state.saveFileLoaded();
        }
    } catch (std::exception &e) {
        error_message_->showMessage(e.what());
    }
}

void MainWindow::savePlan() {
    try {
        auto        save_file      = state.toStationSaveFile();
        std::string save_file_name = save_file.name;
        std::ranges::transform(save_file_name, save_file_name.begin(), [](auto c) {
            if (std::isspace(c))
                return '_';
            return static_cast<char>(std::tolower(c));
        });

        QFileDialog dialog(this, "Save file", QString::fromStdString(fmt::format("{}.json", save_file_name)));

        if (dialog.exec()) {
            auto selected_file = dialog.selectedFiles().first();
            rfl::json::save(selected_file.toStdString(), save_file);
        }
    } catch (std::exception &e) {
        error_message_->showMessage(e.what());
    }
}

void MainWindow::complexUpdated() {
    spdlog::debug("Complex update triggered");
    this->update();
    summary_section_->updateTargetList(state.complex());
}
