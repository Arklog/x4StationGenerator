add_library(x4StationGeneratorExtractor STATIC
        models/modules.cpp
        models/modules.hpp
        models/modules_xsd.hpp
        models/modulegroups.cpp
        models/modulegroups.hpp
        models/modulegroups_xsd.hpp
        models/t.cpp
        models/t.hpp
        models/t_xsd.hpp
        models/wares.cpp
        models/wares.hpp
        models/wares_xsd.hpp
        Extractor.cpp
        Extractor.hpp
        ExtractorSettings.hpp
        ExtractorSettings.cpp
        utils.cpp
        utils.hpp
        CacheFile.cpp
        CacheFile.hpp
        LangFile.cpp
        LangFile.hpp
)
target_include_directories(x4StationGeneratorExtractor PRIVATE .)
target_link_libraries(x4StationGeneratorExtractor PUBLIC spdlog::spdlog nlohmann_json)

add_executable(x4StationGeneratorExtractorBin main.cpp)
target_link_libraries(x4StationGeneratorExtractorBin PRIVATE x4StationGeneratorExtractor)