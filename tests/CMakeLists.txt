add_executable(x4StationGenerator_tests
        smoke.cpp
)
target_link_libraries(x4StationGenerator_tests PRIVATE
        Catch2::Catch2WithMain
)

add_test(NAME x4StationGenerator_tests COMMAND x4StationGenerator_tests)
