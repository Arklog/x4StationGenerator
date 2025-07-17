#include <QApplication>
#include "ui/mainwindow.h"
#include "Data/Loader.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Loader loader{};
    loader.load();

    MainWindow w;
    w.show();
    return QApplication::exec();
}
