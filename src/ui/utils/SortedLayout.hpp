//
// Created by pierre on 7/20/26.
//

#ifndef X4STATIONGENERATOR_SRC_UI_UTILS_SORTEDLAYOUT_HPP
#define X4STATIONGENERATOR_SRC_UI_UTILS_SORTEDLAYOUT_HPP
#include <QLayout>
#include <utility>
#include <QWidget>

namespace ui::utils {
    template<typename T, typename V>
    concept comparator_fn = requires(const V *v1, const V *v2)
    {
        { T()(v1, v2) } -> std::convertible_to<bool>;
        std::is_base_of_v<QWidget, V>;
    };

    template<typename VType, comparator_fn<VType> Fn, typename LayoutType = QVBoxLayout, typename ContainerType =
        std::vector<VType *> >
    class SortedLayout {
    public:
        using value_type = VType;

        LayoutType *  layout;
        ContainerType container;

        SortedLayout(QWidget *parent = nullptr) :
        layout(new LayoutType(parent)),
        container{},
        _parent(parent) {
        };

        void insert(VType *vtype) {
            container.push_back(vtype);
            std::ranges::sort(container, Fn());

            layout->deleteLater();
            layout = new LayoutType(_parent);
            std::ranges::for_each(container, [&](auto v) {
                layout->addWidget(v);
            });
        }

        template<std::ranges::range Container>
        void insert_range(Container &container_) {
            container.insert(container.end(), container_.begin(), container_.end());

            std::ranges::sort(container, Fn());

            layout->deleteLater();
            layout = new LayoutType(_parent);
            std::ranges::for_each(container, [&](auto v) {
                layout->addWidget(v);
            });
        }

        template<typename... Args>
        void emplace(Args &&... args) {
            auto item = new VType(std::forward<Args>(args)...);
            insert(item);
        }

        template<std::ranges::range Container>
        void emplace_range(Container &container_) {
            std::ranges::for_each(container_, [&](auto v) {
                auto item = new VType(std::forward<decltype(v)>(v));
                container.emplace_back(item);
            });

            std::ranges::sort(container, Fn());

            layout->deleteLater();
            layout = new LayoutType(_parent);
            std::ranges::for_each(container, [&](auto v) {
                layout->addWidget(v);
            });
        }

    private:
        QWidget *_parent;
    };
}

#endif //X4STATIONGENERATOR_SRC_UI_UTILS_SORTEDLAYOUT_HPP
