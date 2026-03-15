add_library(test_utils STATIC
    test_utils.cpp
    ComplexInfoView.cpp
)
target_link_libraries(
        test_utils PUBLIC
        spdlog::spdlog
        x4StationGeneratorLib
)
target_include_directories(
        test_utils PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)