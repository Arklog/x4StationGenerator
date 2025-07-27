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
)
target_include_directories(x4StationGenerator
        INTERFACE $<BUILD_INTERFACE:${AUTOGEN_INCLUDE_DIR}>
)
target_include_directories(x4StationGenerator PRIVATE src)

target_link_libraries(x4StationGenerator PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        nlohmann_json::nlohmann_json
        spdlog::spdlog
)

