set(TEST_JSON_MODELS
        "test/data/modules/json/prod_module_01.json"
)
set(TEST_JSON_MODELS_CONTENT)

foreach (item ${TEST_JSON_MODELS})
    file(READ ${item} out_item)
    string(REPLACE "\"" "\\\"" out_item ${out_item})
    cmake_path(GET item STEM FILE_STEM)
    string(TOUPPER ${FILE_STEM} FILE_STEM)
    set(TEST_JSON_MODELS_CONTENT ${TEST_JSON_MODELS_CONTENT} ${FILE_STEM}="${out_item}")
endforeach ()
message(${TEST_JSON_MODELS_CONTENT})

add_library(libtest STATIC
        test/data/Store.cpp
        test/data/modules/Modules.cpp
        test/data/wares/Wares.cpp
        test/data/wares/Wares.hpp
)
target_link_libraries(libtest PUBLIC x4StationGeneratorCommon x4StationGeneratorExtractor x4StationGeneratorUi)
target_include_directories(libtest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(libtest PRIVATE ${TEST_JSON_MODELS_CONTENT})