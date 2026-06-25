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
)
target_include_directories(x4StationGeneratorExtractor PRIVATE .)

add_executable(x4StationGeneratorExtractorBin main.cpp)
target_link_libraries(x4StationGeneratorExtractorBin PRIVATE x4StationGeneratorExtractor)