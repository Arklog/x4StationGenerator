//
// Created by pierre on 7/15/26.
//

#ifndef X4STATIONGENERATOR_SRC_LIBCOMMON_DATA_AGGREGATE_HPP
#define X4STATIONGENERATOR_SRC_LIBCOMMON_DATA_AGGREGATE_HPP
#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/types/Workforce.hpp"
#include "common/types/module/ModuleWrapper.hpp"
#include "common/types/module/ProductionModule.hpp"

namespace common::data {
    template<typename T>
    struct Aggregate {
        using data_type = T;
        std::deque<T>                        datas;
        std::unordered_map<std::string, T *> by_id;
        std::unordered_map<std::string, T *> by_name;

        T &add(T &&data) {
            auto &item                       = datas.emplace_back(std::move(data));
            by_id[item.module.value().id]     = &item;
            by_name[item.module.value().name] = &item;
            return item;
        }
    };

    template<>
    struct Aggregate<types::module::ProductionModule> {
        using data_type = types::module::ProductionModule;
        std::deque<types::module::ProductionModule>                                               datas;
        std::unordered_map<std::string, types::module::ProductionModule *>                        by_id;
        std::unordered_map<std::string, types::module::ProductionModule *>                        by_name;
        std::unordered_map<types::Ware::ware_id, std::vector<types::module::ProductionModule *> > producing;

        types::module::ProductionModule &add(data_type &&value) {
            datas.emplace_back(std::move(value));
            auto &module                        = datas.back();
            by_id[module.module.value().id]     = &datas.back();
            by_name[module.module.value().name] = &datas.back();

            for (auto &[wareid, amount]: module.wares_produced) {
                producing[wareid].emplace_back(&module);
            }

            return module;
        }
    };

    template<>
    struct Aggregate<types::Ware> {
        using data_type = types::Ware;
        std::deque<types::Ware>                                                     datas;
        std::unordered_map<std::string, types::Ware *>                              by_id;
        std::unordered_map<std::string, types::Ware *>                              by_name;
        std::unordered_map<size_t, std::vector<types::Ware *> >                     by_tier;
        std::unordered_map<types::Ware::ware_group_id, std::vector<types::Ware *> > by_waregroup;

        types::Ware &add(types::Ware &&data) {
            auto &item         = datas.emplace_back(std::move(data));
            by_id[item.id]     = &item;
            by_name[item.name] = &item;
            by_tier[item.tier].emplace_back(&item);
            by_waregroup[item.group].emplace_back(&item);
            return item;
        }
    };

    template<>
    struct Aggregate<types::Workforce> {
        using data_type = types::Workforce;
        std::deque<types::Workforce>                        datas;
        std::unordered_map<std::string, types::Workforce *> by_id;

        types::Workforce &add(types::Workforce &&data) {
            auto &item       = datas.emplace_back(std::move(data));
            by_id[item.race] = &item;
            return item;
        }
    };

    template<>
    struct Aggregate<types::module::ModuleWrapper> {
        std::deque<types::module::ModuleWrapper>                        datas;
        std::unordered_map<std::string, types::module::ModuleWrapper *> by_id;
        std::unordered_map<std::string, types::module::ModuleWrapper *> by_name;

        types::module::ModuleWrapper &add(types::module::ModuleWrapper &&m) {
            auto &item                       = datas.emplace_back(std::move(m));
            by_id[item.module.get().id]     = &item;
            by_name[item.module.get().name] = &item;
            return item;
        }
    };
}

#endif //X4STATIONGENERATOR_SRC_LIBCOMMON_DATA_AGGREGATE_HPP
