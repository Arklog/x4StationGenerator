cmake_minimum_required(VERSION 3.28)
project(x4StationGenerator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(
        src
        cmake-build-debug/x4StationGenerator_autogen/include
)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(x4StationGenerator
        main.cpp
        moduleselector.h moduleselector.cpp moduleselector.ui
        mainwindow.h mainwindow.cpp mainwindow.ui
        src/ModuleGenerator.hpp
        src/modules.cpp
        src/ModuleGenerator.cpp
        src/StationBuilder/StationBuilder.hpp
        src/StationBuilder/StationBuilder.cpp
)
target_link_libraries(x4StationGenerator
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)

