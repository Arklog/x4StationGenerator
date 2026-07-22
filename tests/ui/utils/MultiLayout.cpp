#include "catch2/catch_test_macros.hpp"
//
// Created by pierre on 7/20/26.
//
#include "utils/MultiLayout.hpp"

#include <QApplication>
#include <QLabel>

#include "utils/SortedLayout.hpp"

TEST_CASE("test") {
    int          a = 0;
    QApplication app(a, nullptr);

    struct AsciiCmp {
        bool operator()(const QLabel *a, const QLabel *b) {
            return a->text() < b->text();
        }
    };
    auto ascii = [](const QLabel *l1, const QLabel *l2) {
        return l1->text() < l2->text();
    };
    auto by_size = [](const QLabel *l1, const QLabel *l2) {
        return l1->text().size() < l2->text().size();
    };
    using ascii_layout = ui::utils::SortedLayout<QLabel, AsciiCmp>;
    using size_layout  = ui::utils::SortedLayout<QLabel, decltype(by_size)>;

    ascii_layout           l0{};
    size_layout            l1{};
    ui::utils::MultiLayout multi_layout{l0, l1};
    auto &                 l0ref = multi_layout.get<0>();
    auto &                 l1ref = multi_layout.get<1>();

    multi_layout.emplace("0");
    REQUIRE(l0ref.container.size() == 1);
    REQUIRE(l1ref.container.size() == 1);

    std::vector v0{"a"};
    multi_layout.emplace_range(v0);
    REQUIRE(l0ref.container.size() == 2);
    REQUIRE(l1ref.container.size() == 2);

    multi_layout.emplace("test");
    REQUIRE(l0ref.container.size() == 3);
    REQUIRE(l1ref.container.size() == 3);

    std::vector v1{"8"};
    multi_layout.emplace_range(v1);
    REQUIRE(l0ref.container.size() == 4);
    REQUIRE(l1ref.container.size() == 4);
}
