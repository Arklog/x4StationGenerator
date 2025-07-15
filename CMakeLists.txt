cmake_minimum_required(VERSION 3.28)
project(x4StationGenerator)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(
        json
)
#
#FetchContent_Declare(
#        spdlog
#        GIT_REPOSITORY https://github.com/gabime/spdlog
#        GIT_TAG v1.15.3
#)
#FetchContent_MakeAvailable(
#        spdlog
#)

#set(CMAKE_CXX_COMPILER ccache)
#if (WIN32)
#    set(CMAKE_CXX_COMPILER_LAUNCHER sccache)
#else ()
#    set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
#endif ()

include_directories(
        src
        cmake-build-debug/include
)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

file(GLOB_RECURSE SOURCES src/*.cpp src/**/*.cpp widgets/*.cpp widgets/*.ui)
include_directories(src widgets cmake-build-debug/x4StationGenerator_autogen/include)

add_executable(x4StationGenerator
        main.cpp ${SOURCES}
        src/Data/Loader.cpp
        src/Data/Loader.h
)

target_link_libraries(x4StationGenerator PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        nlohmann_json::nlohmann_json
#        spdlog::spdlog
)

