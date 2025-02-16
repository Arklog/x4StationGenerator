cmake_minimum_required(VERSION 3.28)
project(x4StationGenerator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

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

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

file(GLOB_RECURSE SOURCES src/*.cpp widgets/*.cpp widgets/*.ui)
include_directories(src widgets cmake-build-debug/x4StationGenerator_autogen/include)

add_executable(x4StationGenerator
        main.cpp ${SOURCES}
)

target_link_libraries(x4StationGenerator
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)

