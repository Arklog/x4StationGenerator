add_library(x4StationGeneratorCommon STATIC
        common/types/Module.hpp
        common/types/Module.cpp)
target_link_libraries(x4StationGeneratorCommon PRIVATE nlohmann_json spdlog::spdlog)
target_include_directories(x4StationGeneratorCommon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})