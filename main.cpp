#include "Data/Loader.hpp"
#include "Data/WareModuleAndWorkforce.hpp"
#include "ui/mainwindow.h"
#include <QApplication>

#include "spdlog/spdlog.h"

int main (int argc, char *argv[])
{
    QApplication a (argc, argv);

#ifdef NDEBUG
    spdlog::set_level (spdlog::level::info);
#else
    spdlog::set_level (spdlog::level::debug);
#endif
    Store store;
    Loader loader{store};
    loader.load ();
    buildDataFrom (loader._modules_json);
    buildDataFrom (loader._workforce);

    MainWindow w{store, nullptr};
    w.show ();
    return QApplication::exec ();
}
