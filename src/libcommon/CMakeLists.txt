add_library(x4StationGeneratorCommon STATIC
        common/types/Module.hpp
        common/types/Module.cpp
        data/Data.cpp
        data/Data.hpp
        data/ID.cpp
        data/ID.h
        data/Loader.cpp
        data/Loader.hpp
        data/Store.cpp
        data/Store.hpp
        data/WareModuleAndWorkforce.cpp
        data/WareModuleAndWorkforce.hpp
        stationbuilder/Generator/ComplexGeneratorBase.cpp
        stationbuilder/Generator/ComplexGeneratorBase.hpp
        stationbuilder/defines.cpp
        stationbuilder/defines.hpp
        utils/FmtVectorFormatter.hpp
        utils/WareTargetContainer.cpp
        utils/WareTargetContainer.hpp
        ThreadPool.cpp
        ThreadPool.hpp)
target_link_libraries(x4StationGeneratorCommon PRIVATE nlohmann_json spdlog::spdlog)
target_include_directories(x4StationGeneratorCommon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})