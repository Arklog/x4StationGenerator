#include <QApplication>
#include "ui/mainwindow.h"
#include "Data/Loader.hpp"
#include "Data/Modules.hpp"
#include "Data/Wares.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Loader loader{};
    loader.load();
    setWares(loader._wares_json);
    setModules(loader._modules_json);

    MainWindow w;
    w.show();
    return QApplication::exec();
}
