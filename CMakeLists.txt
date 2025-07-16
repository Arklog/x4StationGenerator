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

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(
        src
        cmake-build-debug/x4StationGenerator_autogen/include
)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

file(GLOB_RECURSE SOURCES src/*.cpp src/**/*.cpp src/ui/*.cpp src/ui/*.ui)
include_directories(src src/ui cmake-build-debug/x4StationGenerator_autogen/include)

add_executable(x4StationGenerator
        main.cpp ${SOURCES}
        src/Data/Loader.cpp
        src/Data/Loader.hpp
        src/Data/Data.cpp
        src/Data/Data.hpp
        src/Data/Wares.cpp
        src/Data/Wares.hpp
        src/ui/section/wareselectionsection.cpp
        src/ui/section/wareselectionsection.h
        src/ui/section/wareselectionsection.ui
)

target_link_libraries(x4StationGenerator PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        nlohmann_json::nlohmann_json
        spdlog::spdlog
)

