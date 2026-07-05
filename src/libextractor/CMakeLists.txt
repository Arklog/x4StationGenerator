add_library(x4StationGeneratorExtractor STATIC
        extractor/models/modules.cpp
        extractor/models/modules.hpp
        extractor/models/modules_xsd.hpp
        extractor/models/modulegroups.cpp
        extractor/models/modulegroups.hpp
        extractor/models/modulegroups_xsd.hpp
        extractor/models/t.cpp
        extractor/models/t.hpp
        extractor/models/t_xsd.hpp
        extractor/models/wares.cpp
        extractor/models/wares.hpp
        extractor/models/wares_xsd.hpp
        extractor/Extractor.cpp
        extractor/Extractor.hpp
        extractor/ExtractorSettings.hpp
        extractor/ExtractorSettings.cpp
        extractor/utils.cpp
        extractor/utils.hpp
        extractor/CacheFile.cpp
        extractor/CacheFile.hpp
        extractor/LangFile.cpp
        extractor/LangFile.hpp
        extractor/Store.hpp
        extractor/extraction_logic/Extension.cpp
        extractor/extraction_logic/Extension.hpp
        extractor/extraction_logic/Archive.cpp
        extractor/extraction_logic/Archive.hpp
        extractor/extraction_logic/X4Extractable.hpp
        extractor/extraction_logic/X4Extractable.cpp
        extractor/extraction_logic/FileDetector.cpp
        extractor/extraction_logic/FileDetector.hpp
)
target_include_directories(x4StationGeneratorExtractor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(x4StationGeneratorExtractor PUBLIC spdlog::spdlog nlohmann_json x4StationGeneratorCommon)

add_executable(x4StationGeneratorExtractorBin extractor/main.cpp)
target_link_libraries(x4StationGeneratorExtractorBin PRIVATE x4StationGeneratorExtractor)