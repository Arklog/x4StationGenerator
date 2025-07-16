#include <QApplication>
#include <QPushButton>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <iostream>
#include "mainwindow.h"
#include "modules.hpp"
#include "Data/Loader.hpp"
#include "Data/Wares.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Loader loader{};
    loader.load();
    setWares(loader._wares_json);

    MainWindow w;
    w.show();
    return QApplication::exec();
}
