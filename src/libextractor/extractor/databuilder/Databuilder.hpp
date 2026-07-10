//
// Created by pierre on 7/9/26.
//

#ifndef X4STATIONGENERATOR__DATABUILDER_HPP
#define X4STATIONGENERATOR__DATABUILDER_HPP
#include <vector>

#include "extractor/databuilder/AggregateStore.hpp"
#include "../../../libcommon/common/types/module/Module.hpp"

namespace extractor::databuilder {
    struct Databuilder {
        Databuilder(AggregateStore &&store);

        std::vector<common::Module> modules;
    };
} // extractor

#endif //X4STATIONGENERATOR__DATABUILDER_HPP
