#include <QApplication>
#include "ui/mainwindow.h"
#include "Data/Loader.hpp"
#include "Data/WaresAndModules.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Loader loader{};
    loader.load();
    buildDataFrom(loader._modules_json);

    MainWindow w;
    w.show();
    return QApplication::exec();
}
