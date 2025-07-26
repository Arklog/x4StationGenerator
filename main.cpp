#include <QApplication>
#include "ui/mainwindow.h"
#include "Data/Loader.hpp"
#include "Data/WareModuleAndWorkforce.hpp"

#include "spdlog/spdlog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

# ifdef DEBUG
    spdlog::set_level(spdlog::level::debug);
# endif

    Loader loader{};
    loader.load();
    buildDataFrom(loader._modules_json);

    MainWindow w;
    w.show();
    return QApplication::exec();
}

