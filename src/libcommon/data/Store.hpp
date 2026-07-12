//
// Created by pierre on 3/31/26.
//

#ifndef X4STATIONGENERATOR__STORE_HPP
#define X4STATIONGENERATOR__STORE_HPP

#include "common/types/Workforce.hpp"
#include "common/types/module/Dock.hpp"
#include "common/types/module/Habitat.hpp"
#include "common/types/module/Pierr.hpp"
#include "common/types/module/ProductionModule.hpp"
#include "common/types/module/Storage.hpp"

namespace common::data {
    struct Store {
        std::vector<common::types::module::Dock>    docks;
        std::vector<common::types::module::Pierr>   piers;
        std::vector<common::types::module::Habitat> habitats;
        std::vector<common::types::module::Storage> storages;

        template<typename T>
        struct Aggregate {
            using data_type = T;
            std::vector<T>                       datas;
            std::unordered_map<std::string, T *> by_id;
            std::unordered_map<std::string, T *> by_name;

            virtual void add(T &&data) {
                datas.emplace_back(std::move(data));
                by_id[data.module.value().id]     = &datas.back();
                by_name[data.module.value().name] = &datas.back();
            }
        };

        struct {
            std::vector<common::types::Workforce>                             workforces;
            std::unordered_map<types::Workforce::race_id, types::Workforce *> by_race;
        } workforce;

        struct : Aggregate<types::module::ProductionModule> {
            std::unordered_map<types::Ware::ware_id, std::vector<types::module::ProductionModule *> > producing;

            void add(data_type &&value) override {
                Aggregate::add(std::move(value));
                auto &module = datas.back();

                for (auto &[wareid, amount]: module.wares_produced) {
                    producing[wareid].emplace_back(&module);
                }
            }
        } production;

        Aggregate<types::Ware> wares;
    };
}

#endif // X4STATIONGENERATOR__STORE_HPP
