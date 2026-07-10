add_library(x4StationGeneratorCommon STATIC
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
        ThreadPool.hpp
        common/types/Price.cpp
        common/types/Price.hpp
        common/types/Ware.cpp
        common/types/Ware.hpp
        common/types/module/Module.hpp
        common/types/module/Module.cpp
        common/types/module/ProductionModule.cpp
        common/types/module/ProductionModule.hpp
        common/types/module/Habitat.cpp
        common/types/module/Habitat.hpp
        common/types/Workforce.cpp
        common/types/Workforce.hpp
        common/types/module/Dock.cpp
        common/types/module/Dock.hpp
        common/types/module/Pierr.cpp
        common/types/module/Pierr.hpp)
target_link_libraries(x4StationGeneratorCommon PRIVATE nlohmann_json spdlog::spdlog)
target_include_directories(x4StationGeneratorCommon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})