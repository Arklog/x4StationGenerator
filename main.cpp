#include <QApplication>
#include <QPushButton>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <iostream>
#include "mainwindow.h"
#include "modules.hpp"
#include "Data/Loader.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Loader loader{};
    loader.load();

    MainWindow w;
    w.show();
    return QApplication::exec();
}
