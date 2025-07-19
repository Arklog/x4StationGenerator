cmake_minimum_required(VERSION 3.28)
project(x4StationGenerator)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)


FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(
        json
)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.15.3
)
FetchContent_MakeAvailable(
        spdlog
)


#set(CMAKE_CXX_COMPILER ccache)
#set(CMAKE_C_COMPILER ccache)
if (WIN32)
    set(CMAKE_CXX_COMPILER_LAUNCHER sccache)
else ()
    set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
    set(CMAKE_C_COMPILER_LAUNCHER ccache)
endif ()

get_property(_isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if (_isMultiConfig)
    set(AUTOGEN_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_autogen/include_$<CONFIG>)
else ()
    set(AUTOGEN_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_autogen/include)
endif ()

include_directories(src cmake-build-debug/x4StationGenerator_autogen/include)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(x4StationGenerator
        main.cpp
        src/modules.cpp
        src/BuildSettings.cpp
        src/ui/mainwindow.cpp
        src/ui/widgets/wareconfiguratorpanel.ui
        src/ui/widgets/waresselector.ui
        src/ui/widgets/wareconfiguratorpanel.cpp
        src/ui/widgets/wareconfigurator.cpp
        src/ui/widgets/waresselector.cpp
        src/ui/widgets/stationsizewidget.cpp
        src/ui/widgets/wareconfigurator.ui
        src/ui/widgets/moduleselectorwidget.ui
        src/ui/widgets/moduleselectorwidget.cpp
        src/ui/widgets/stationsizewidget.ui
        src/ui/section/wareselectionsection.cpp
        src/ui/section/settingssection.cpp
        src/ui/section/settingssection.ui
        src/ui/section/wareselectionsection.ui
        src/ui/section/ressourceproducedsection.cpp
        src/ui/section/modulessection.ui
        src/ui/section/ressourceproducedsection.ui
        src/ui/section/buildsummarysection.ui
        src/ui/section/buildsummarysection.cpp
        src/ui/section/modulessection.cpp
        src/ui/mainwindow.ui
        src/utils.cpp
        src/StationBuilder/StationBuilder.cpp
        src/Data/WaresAndModules.cpp
        src/Data/Loader.cpp
        src/Data/Data.cpp
)

target_include_directories(x4StationGenerator INTERFACE
        $<BUILD_INTERFACE:${AUTOGEN_INCLUDE_DIR}>
)

target_link_libraries(x4StationGenerator PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        nlohmann_json::nlohmann_json
        spdlog::spdlog
)

