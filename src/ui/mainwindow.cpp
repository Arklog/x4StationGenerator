#include "mainwindow.h"
#include <QFileDialog>
#include <QTextStream>
#include <cmath>

#include "modules.hpp"

#include "ui_mainwindow.h"

#include "section/DockAndPierrSection/dockandpierrsection.hpp"
#include "section/SettingsSection/settingssection.hpp"
#include "section/StorageSelectionSection/storagesection.hpp"
#include "section/SummarySection/summarysection.hpp"
#include "section/WareSelectionSection/wareselectionsection.h"

#include "spdlog/spdlog.h"

MainWindow::MainWindow (const Store &store, QWidget *parent)
    : QMainWindow (parent), ui (new Ui::MainWindow), settings_{},
      store_ (store), complex_{}
{
    ui->setupUi (this);

    auto ware_selection_section
	= new WareSelectionSection (settings_, store_, this);
    auto dock_and_pierr_section = new DockAndPierrSection (store, this);
    auto storage_section = new StorageSection (store, this);
    auto settings_section = new SettingsSection (settings_, this);
    auto summary_section = new SummarySection (store, this);

    ui->ware_selection_tab_layout->addWidget (ware_selection_section);
    ui->dock_and_pierr_tab_layout->addWidget (dock_and_pierr_section);
    ui->storage_tab_layout->addWidget (storage_section);
    ui->summary_tab_layout->addWidget (summary_section);
    ui->settings_tab_layout->addWidget (settings_section);

    this->ware_selection_section_ = ware_selection_section;
    this->dock_and_pierr_section_ = dock_and_pierr_section;
    this->storage_section_ = storage_section;
    this->summary_section_ = summary_section;
    this->settings_section_ = settings_section;

    connect (ware_selection_section_, &WareSelectionSection::complexUpdated,
	     this, &MainWindow::complexUpdated);
    connect (storage_section_, &StorageSection::storageUpdated, this,
	     &MainWindow::complexUpdated);
    connect (dock_and_pierr_section_, &DockAndPierrSection::dockAndPierrUpdated,
	     this, &MainWindow::complexUpdated);
    connect (ui->action_export, &QAction::triggered, this,
	     &MainWindow::exportPlan);
}

MainWindow::~MainWindow () { delete ui; }

void MainWindow::exportPlan ()
{
    spdlog::info ("Exporting plan");

    QFileDialog dialog (this);
    dialog.setFileMode (QFileDialog::FileMode::AnyFile);
    dialog.setAcceptMode (QFileDialog::AcceptMode::AcceptSave);

    if (dialog.exec ())
    {
	auto selected_file = dialog.selectedFiles ().first ();
	spdlog::info ("selected file: {0}", selected_file.toStdString ());

	QFile file (selected_file);

	// non empty file, check with user
	// if (file.size() != 0) {
	// spdlog::info("non empty file, waiting for confirmation");
	// QDialog dialog(this);
	// dialog.setWindowFlags(Qt::Dialog);
	// dialog.setModal(true);
	// dialog.setWindowTitle("Non empty file");
	// dialog.exec();

	// bool ok = false;
	// connect(dialog, &QDialog::accept, [&ok] () -> void { ok = true; });

	// spdlog::info("file was accepted");
	// }

	if (file.open (QIODevice::WriteOnly))
	{
	    QTextStream stream (&file);
	    auto data = genModulePlan (complex_, store_, settings_);

	    file.write (data.c_str ());
	}
    }
}

void MainWindow::complexUpdated ()
{
    spdlog::debug ("Complex update triggered");
    this->update ();

    complex_.clear ();
    const auto &base_complex = this->ware_selection_section_->getComplex ();
    const auto &storages = this->storage_section_->getModuleTargetList ();
    const auto &dock_and_pierr
	= this->dock_and_pierr_section_->getModuleTargetList ();

    spdlog::debug ("Inserting {} dock and pierr modules",
		   dock_and_pierr.size ());
    for (const auto &[module_id, amount] : dock_and_pierr)
    {
	for (size_t i = 0; i < amount; ++i)
	    complex_.push_back (module_id);
    }

    spdlog::debug ("Inserting {} storage modules", storages.size ());
    for (const auto &[module_id, amount] : storages)
    {
	for (size_t i = 0; i < amount; ++i)
	    complex_.push_back (module_id);
    }

    spdlog::debug ("Inserting {} modules", base_complex.size ());
    complex_.insert (complex_.end (), base_complex.begin (),
		     base_complex.end ());

    summary_section_->updateTargetList (complex_);
}
