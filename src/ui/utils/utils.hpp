//
// Created by pierre on 7/21/26.
//

#ifndef X4STATIONGENERATOR_SRC_UI_UTILS_UTILS_HPP
#define X4STATIONGENERATOR_SRC_UI_UTILS_UTILS_HPP
#include <algorithm>
#include <QLayout>
#include <ranges>

class QLayout;

void clearLayout(QLayout *layout);

void clearLayoutNoDelete(QLayout *layout);

void copyLayout(QLayout *src, QLayout *dst);

template<std::ranges::range ContainerType>
void copyToLayout(QLayout *dest, const ContainerType &src) {
    static_assert(std::is_base_of_v<QWidget, std::remove_pointer_t<typename ContainerType::value_type> >,
                  "Container value type must descend from QWidget");
    static_assert(std::is_pointer_v<typename ContainerType::value_type>, "Container value type must be a pointer");

    clearLayoutNoDelete(dest);
    std::ranges::for_each(src, [&](auto item) {
        dest->addWidget(item);
    });
}

#endif //X4STATIONGENERATOR_SRC_UI_UTILS_UTILS_HPP
