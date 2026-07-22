cmake_minimum_required(VERSION 3.28)
project(x4StationGenerator)

set(CMAKE_CXX_STANDARD 23)

set(REFLECTCPP_XML ON)
set(REFLECTCPP_JSON ON)

#################################################################################
# External Libraries
#################################################################################

include(FetchContent)

FetchContent_Declare(
        pugixml
        GIT_REPOSITORY https://github.com/zeux/pugixml.git
        GIT_TAG v1.16
)
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

FetchContent_Declare(
        reflect_cpp
        GIT_REPOSITORY https://github.com/getml/reflect-cpp.git
        GIT_TAG v0.25.0
)

FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.15.2
)

FetchContent_MakeAvailable(
        pugixml
        spdlog
        argparse
        nlohmann_json
        reflect_cpp
        catch2
)

#################################################################################
# Qt6
#################################################################################

#if (WIN32)
#    set(CMAKE_CXX_COMPILER_LAUNCHER sccache)
#else ()
#    find_program(CCACHE_PROGRAM ccache)
#    if (CCACHE_PROGRAM STREQUAL "CCACHE_PROGRAM-NOTFOUND")
#        message(WARNING "ccache not found!")
#    else ()
#        message(STATUS "ccache found: ${CCACHE_PROGRAM}")
#        set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
#        set(CMAKE_C_COMPILER_LAUNCHER ccache)
#    endif ()
#endif ()


if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(DEBUG_COMPILE_FLAGS -O0 -g -fno-dce -fno-dse -fno-tree-dce -fno-tree-dse -fno-inline -fconcepts-diagnostics-depth=3)
    message(STATUS "DEBUG_COMPILE_FLAGS: ${DEBUG_COMPILE_FLAGS}")
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

add_subdirectory(src/libcommon)
add_subdirectory(src/libextractor)
add_subdirectory(src/ui)

add_executable(x4StationGenerator main.cpp)
target_compile_options(x4StationGenerator PRIVATE ${DEBUG_COMPILE_FLAGS})
target_link_libraries(x4StationGenerator PRIVATE
        x4StationGeneratorCommon
        x4StationGeneratorExtractor
        x4StationGeneratorUi
)

enable_testing()

function(create_test test_name test_file required_lib)
    add_executable(${test_name} ${test_file})
    target_link_libraries(${test_name} PRIVATE Catch2::Catch2WithMain ${required_lib})
    target_compile_options(${test_name} PRIVATE ${DEBUG_COMPILE_FLAGS})
    add_test(${test_name} ${test_name})
endfunction()

add_subdirectory(tests)