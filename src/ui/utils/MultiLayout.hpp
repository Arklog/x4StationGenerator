//
// Created by pierre on 7/20/26.
//

#ifndef X4STATIONGENERATOR_SRC_UI_UTILS_MULTILAYOUT_HPP
#define X4STATIONGENERATOR_SRC_UI_UTILS_MULTILAYOUT_HPP
#include <tuple>
#include <type_traits>

namespace ui::utils {
    template<typename VType, typename... Args>
    concept contain_value = requires(Args... args)
    {
        (... && std::is_same<VType, typename Args::value_type>::value);
    };

    template<typename T, typename... Args>
    class MultiLayout {
    public:
        std::tuple<Args...> layouts;

        MultiLayout() :
        layouts(Args()...) {
        }

        MultiLayout(Args... args) :
        layouts(args...) {
        }

        template<size_t N>
        auto get() -> decltype(std::get<N>(layouts)) {
            return std::get<N>(layouts);
        }

        void insert(T *item) {
            std::apply([&](auto &... layout) -> decltype(auto) {
                (..., layout.insert(item));
            }, layouts);
        }

        template<std::ranges::range Container>
        void insert_range(Container &container) {
            std::apply([&](auto &... layout) -> decltype(auto) {
                (..., layout.insert_range(container));
            }, layouts);
        }

        template<typename... VtypeArgs>
        void emplace(VtypeArgs... args) {
            std::apply([&](auto &... layout) -> decltype(auto) {
                (..., layout.emplace(args...));
            }, layouts);
        }

        template<std::ranges::range Container>
        void emplace_range(Container &container) {
            std::apply([&](auto &... layout) -> decltype(auto) {
                (..., layout.emplace_range(container));
            }, layouts);
        }
    };
}

#endif //X4STATIONGENERATOR_SRC_UI_UTILS_MULTILAYOUT_HPP
