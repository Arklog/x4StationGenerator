add_library(x4StationGeneratorExtractor STATIC
        extractor/Extractor.cpp
        extractor/Extractor.hpp
        extractor/ExtractorSettings.hpp
        extractor/ExtractorSettings.cpp
        extractor/utils.cpp
        extractor/utils.hpp
        extractor/CacheFile.cpp
        extractor/CacheFile.hpp
        extractor/Store.hpp
        extractor/extraction_logic/Extension.cpp
        extractor/extraction_logic/Extension.hpp
        extractor/extraction_logic/Archive.cpp
        extractor/extraction_logic/Archive.hpp
        extractor/extraction_logic/X4Extractable.hpp
        extractor/extraction_logic/X4Extractable.cpp
        extractor/extraction_logic/FileDetector.cpp
        extractor/extraction_logic/FileDetector.hpp
        extractor/patching_logic/X4Patchable.cpp
        extractor/patching_logic/X4Patchable.hpp
        extractor/models/model_store.cpp
        extractor/models/model_store.hpp
        extractor/models/Structure.cpp
        extractor/models/Structure.hpp
        extractor/databuilder/ModuleAggregator.cpp
        extractor/databuilder/ModuleAggregator.hpp
        extractor/databuilder/DataBuilder.cpp
        extractor/databuilder/DataBuilder.hpp
        extractor/models/Wares.cpp
        extractor/models/Wares.hpp
        extractor/models/T.cpp
        extractor/models/T.hpp
)
target_include_directories(x4StationGeneratorExtractor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(x4StationGeneratorExtractor PUBLIC spdlog::spdlog nlohmann_json reflectcpp x4StationGeneratorCommon)

add_executable(x4StationGeneratorExtractorBin extractor/main.cpp)
target_link_libraries(x4StationGeneratorExtractorBin PRIVATE x4StationGeneratorExtractor)