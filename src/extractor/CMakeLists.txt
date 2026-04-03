add_library(x4StationGeneratorExtractor STATIC
        models/wares.cpp
        models/wares.hpp
        models/wares_xsd.hpp
)
target_include_directories(x4StationGeneratorExtractor PRIVATE .)
target_link_libraries(x4StationGeneratorExtractor PRIVATE xsd)