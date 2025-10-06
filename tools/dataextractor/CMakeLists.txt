#################################################################
# Build target
#################################################################

add_executable(dataextractor
        src/main.cpp
        src/WareExtractor.cpp
        src/WareExtractor.h
)

target_link_libraries(
        dataextractor PRIVATE
        spdlog::spdlog
        argparse::argparse
        nlohmann_json::nlohmann_json
        pugixml
)