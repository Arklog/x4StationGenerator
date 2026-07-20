//
// Created by pierre on 7/20/26.
//
#include "utils/SortedLayout.hpp"

#include <QApplication>
#include <QLabel>
#include <catch2/catch_all.hpp>

TEST_CASE("test") {
    int          argc = 0;
    QApplication app{argc, nullptr};

    auto fn = [](const QLabel *l1, const QLabel *l2) {
        return l1->text().toStdString() < l2->text().toStdString();
    };
    auto layout = ui::utils::SortedLayout<QLabel, decltype(fn)>{};
    auto get_at = [&](int index) {
        auto count  = layout.layout->count();
        auto item   = layout.layout->itemAt(index);
        auto widget = item->widget();
        return dynamic_cast<QLabel *>(widget);
    };

    auto l1 = new QLabel{"0"};
    auto l2 = new QLabel{"1"};
    auto l3 = new QLabel{"2"};

    layout.insert(l1);
    REQUIRE(get_at(0) == l1);

    layout.insert(l3);
    REQUIRE(get_at(0) == l1);
    REQUIRE(get_at(1) == l3);

    layout.insert(l2);
    REQUIRE(get_at(0) == l1);
    REQUIRE(get_at(1) == l2);
    REQUIRE(get_at(2) == l3);

    layout.emplace("5");
    layout.emplace("4");
    REQUIRE(get_at(0) == l1);
    REQUIRE(get_at(1) == l2);
    REQUIRE(get_at(2) == l3);
    REQUIRE(get_at(3)->text() == "4");
    REQUIRE(get_at(4)->text() == "5");

    std::vector<QLabel *> items{new QLabel{"7"}, new QLabel{"6"}};
    layout.insert_range(items);
    REQUIRE(get_at(5)->text() == "6");
    REQUIRE(get_at(6)->text() == "7");

    std::vector items2{"8", "9"};
    layout.emplace_range(items2);
    REQUIRE(get_at(7)->text() == "8");
    REQUIRE(get_at(8)->text() == "9");
}
