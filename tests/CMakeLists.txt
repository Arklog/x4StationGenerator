add_subdirectory(libtest)

function(create_test test_name test_file)
    add_executable(${test_name} ${test_file})
    target_link_libraries(${test_name} PRIVATE Catch2::Catch2WithMain libtest)
    target_compile_options(${test_name} PRIVATE ${DEBUG_COMPILE_FLAGS})
    add_test(${test_name} ${test_name})
endfunction()

add_subdirectory(libcommon)
add_subdirectory(ui)