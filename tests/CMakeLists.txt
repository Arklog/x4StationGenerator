add_library(test_utils SHARED test_utils.cpp)
target_include_directories(test_utils PUBLIC ${CMAKE_SOURCE_DIR}/src .)
target_link_libraries(test_utils PUBLIC x4StationGeneratorLib)

add_subdirectory(StationBuilder)