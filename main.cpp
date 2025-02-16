#include <QApplication>
#include <QPushButton>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <iostream>
#include "mainwindow.h"
#include "modules.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Argument parsing
//    QCommandLineParser parser;
//    parser.setApplicationDescription("X4: Foundation complex generator");
//    parser.addHelpOption();
//
//    QCommandLineOption moduleListOption(
//            QStringList() << "m" << "module-list",
//            "Modules to be generated",
//            "Module name"
//    );
//    parser.addOption(moduleListOption);
//
//    QCommandLineOption moduleNumberOption(
//            QStringList() << "n" << "module-number", "Number of modules", "Number"
//    );
//    parser.addOption(moduleNumberOption);
//
//    parser.process(a);
//
//    if (parser.isSet(moduleListOption) != parser.isSet(moduleNumberOption))
//    {
//        std::cerr << "Both module-list and module-number must be set" << std::endl;
//        return 1;
//    } else if (parser.isSet(moduleListOption) && parser.isSet(moduleNumberOption))
//    {
//        t_modules modules;
//        QString   moduleList = parser.value(moduleListOption);
//        size_t    nmodules   = parser.value(moduleNumberOption).toUInt();
//
//        auto module = std::find_if(
//                MODULES::MODULES.cbegin(),
//                MODULES::MODULES.cend(),
//                [&moduleList](const Module &m) {
//                    std::cout << m.name << std::endl;
//                    return m.name == moduleList.toStdString();
//                }
//        );
//
//        if (module == MODULES::MODULES.cend())
//        {
//            std::cerr << "Module not found" << std::endl;
//            return 1;
//        }
//        modules[*module] = nmodules;
//
//        StationSize    size{.x_plus = 20, .y_plus= 20, .z_plus = 20};
//        StationBuilder stationBuilder(modules, true);
//        auto           plan = genModulePlan("plan", stationBuilder.get(), size);
//        std::cout << plan << std::endl;
//
//        return 0;
//    }

    MainWindow w;
    w.show();
    return QApplication::exec();
}
