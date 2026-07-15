#include "src/libcommon/data/Loader.hpp"
#include "src/libcommon/data/WareModuleAndWorkforce.hpp"
#include "ui/mainwindow.h"
#include <QApplication>

#include "spdlog/spdlog.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

#ifdef NDEBUG
    spdlog::set_level(spdlog::level::info);
#else
    spdlog::set_level(spdlog::level::debug);
#endif
    common::data::Store  store;
    common::data::Loader loader{store, "assets"};
    loader.load();

    MainWindow w{store, nullptr};
    w.show();
    return QApplication::exec();
}
