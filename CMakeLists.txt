cmake_minimum_required(VERSION 3.28)
project(x4StationGenerator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#set(CMAKE_CXX_COMPILER ccache)
if (WIN32)
    set(CMAKE_CXX_COMPILER_LAUNCHER sccache)
else ()
    set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
endif ()

include_directories(
        src
        cmake-build-debug/include
)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(x4StationGenerator
        main.cpp
        widgets/moduleselectorwidget.h widgets/moduleselectorwidget.cpp widgets/moduleselectorwidget.ui
        mainwindow.h mainwindow.cpp mainwindow.ui
        widgets/stationsizewidget.h widgets/stationsizewidget.cpp widgets/stationsizewidget.ui
        widgets/modulessection.hpp widgets/modulessection.cpp widgets/modulessection.ui
        widgets/ressourceproducedsection.hpp widgets/ressourceproducedsection.cpp widgets/ressourceproducedsection.ui
        widgets/buildsummarysection.hpp widgets/buildsummarysection.cpp widgets/buildsummarysection.ui
        src/ModuleGenerator.hpp
        src/modules.cpp
        src/ModuleGenerator.cpp
        src/StationBuilder/StationBuilder.hpp
        src/StationBuilder/StationBuilder.cpp
        src/defines.hpp
)
target_link_libraries(x4StationGenerator
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)

