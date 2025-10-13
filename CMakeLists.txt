cmake_minimum_required(VERSION 3.28)
project(x4StationGenerator)

include(FetchContent)

set(CMAKE_CXX_STANDARD 23)

#################################################################################
# External Libraries
#################################################################################

include(FetchContent)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.15.3
)

FetchContent_Declare(
        argparse
        GIT_REPOSITORY https://github.com/p-ranav/argparse
        GIT_TAG v3.2
)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG v3.12.0
)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.15.3
)

FetchContent_MakeAvailable(
        spdlog
        argparse
        nlohmann_json
)

#################################################################################
# Qt6
#################################################################################

if (WIN32)
    set(CMAKE_CXX_COMPILER_LAUNCHER sccache)
else ()
    find_program(CCACHE_PROGRAM ccache)
    if (CCACHE_PROGRAM STREQUAL "CCACHE_PROGRAM-NOTFOUND")
        message(WARNING "ccache not found!")
    else ()
        message(STATUS "ccache found: ${CCACHE_PROGRAM}")
        set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
        set(CMAKE_C_COMPILER_LAUNCHER ccache)
    endif ()
endif ()

get_property(_isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if (_isMultiConfig)
    set(AUTOGEN_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_autogen/include_$<CONFIG>)
else ()
    set(AUTOGEN_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_autogen/include)
endif ()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(
        Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Charts
        REQUIRED
)

#################################################################################
# Application
#################################################################################

add_executable(x4StationGenerator
        main.cpp
        src/modules.cpp
        src/ui/mainwindow.cpp
        src/ui/section/WareSelectionSection/widgets/wareconfiguratorpanel.ui
        src/ui/section/WareSelectionSection/widgets/waresselector.ui
        src/ui/section/WareSelectionSection/widgets/wareconfiguratorpanel.cpp
        src/ui/section/WareSelectionSection/widgets/wareconfigurator.cpp
        src/ui/section/WareSelectionSection/widgets/waresselector.cpp
        src/ui/section/WareSelectionSection/widgets/wareconfigurator.ui
        src/ui/section/WareSelectionSection/wareselectionsection.cpp
        src/ui/section/WareSelectionSection/wareselectionsection.ui
        src/ui/mainwindow.ui
        src/StationBuilder/Generator/ComplexGeneratorBase.cpp
        src/Data/WareModuleAndWorkforce.cpp
        src/Data/Loader.cpp
        src/Data/Data.cpp
        src/ui/section/DockAndPierrSection/dockandpierrsection.cpp
        src/ui/section/DockAndPierrSection/dockandpierrsection.hpp
        src/ui/section/DockAndPierrSection/dockandpierrsection.ui
        src/ui/widgets/moduleselectionpanel.cpp
        src/ui/widgets/moduleselectionpanel.hpp
        src/ui/widgets/moduleselectionpanel.ui
        src/ui/widgets/moduleconfigurationpanel.cpp
        src/ui/widgets/moduleconfigurationpanel.hpp
        src/ui/widgets/moduleconfigurationpanel.ui
        src/ui/widgets/moduleconfiguration.cpp
        src/ui/widgets/moduleconfiguration.hpp
        src/ui/widgets/moduleconfiguration.ui
        src/ui/section/StorageSelectionSection/storagesection.cpp
        src/ui/section/StorageSelectionSection/storagesection.hpp
        src/ui/section/StorageSelectionSection/storagesection.ui
        src/ui/section/SummarySection/summarysection.cpp
        src/ui/section/SummarySection/summarysection.hpp
        src/ui/section/SummarySection/summarysection.ui
        src/StationBuilder/defines.cpp
        src/ui/section/SettingsSection/settingssection.cpp
        src/ui/section/SettingsSection/settingssection.hpp
        src/ui/section/SettingsSection/settingssection.ui
        src/utils/WareTargetContainer.cpp
        src/utils/WareTargetContainer.hpp
        src/Data/ID.cpp
        src/Data/ID.h
)
target_include_directories(x4StationGenerator
        INTERFACE $<BUILD_INTERFACE:${AUTOGEN_INCLUDE_DIR}>
)
target_include_directories(x4StationGenerator PRIVATE src)

target_link_libraries(x4StationGenerator PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Charts
        nlohmann_json::nlohmann_json
        spdlog::spdlog
)

