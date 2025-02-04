//
// Created by pierre on 2/2/25.
//

#include "StationBuilder.hpp"

StationBuilder::StationBuilder(const t_modules &modules) : _modules(modules) {

}

void StationBuilder::generateBuildOrder(t_module_generator generator) {
    _ordered = generator(_modules);
}

const t_module_list &StationBuilder::get() const {
    return _ordered;
}

t_module_list basic_generator(const t_modules &modules) {
    t_module_list lst;

    for (auto const &module: modules) {
        int i = module.second;
        while (i--)
            lst.push_back(module.first);
    }

    return lst;
}

t_module_list rand_generator(const t_modules &modules) {
    t_module_list                   lst = basic_generator(modules);
    t_module_list                   retv{};
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_int_distribution<> dis;

    while (!lst.empty()) {
        auto i = dis(gen) % lst.size();
        retv.push_back(lst[i]);
        lst.erase(lst.begin() + static_cast<long>(i));
    }

    return retv;
}

t_module_list priority_generator(const t_modules &modules) {
    static const std::vector<MODULE_TYPE> priority = {
            MODULE_TYPE::DOCK,
            MODULE_TYPE::STORAGE,
            MODULE_TYPE::PRODUCTION,
            MODULE_TYPE::HABITAT
    };
    std::map<MODULE_TYPE, t_module_list>  placeholder{
            {MODULE_TYPE::DOCK,       {}},
            {MODULE_TYPE::STORAGE,    {}},
            {MODULE_TYPE::PRODUCTION, {}},
            {MODULE_TYPE::HABITAT,    {}}
    };
    t_module_list                         lst{};

    for (const auto &iter: modules) {
        auto type = iter.first.type;
        auto n    = iter.second;

        while (n--)
            placeholder.at(type).push_back(iter.first);
    }

    for (const auto &type: priority)
        lst.insert(lst.end(), placeholder.at(type).begin(), placeholder.at(type).end());

    return lst;
}
