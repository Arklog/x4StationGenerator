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
    concept comparator_fn = requires(T fn, const V *v1, const V *v2)
    {
        { fn(v1, v2) } -> std::convertible_to<bool>;
        std::is_base_of_v<QWidget, V>;
    };

    template<typename VType, comparator_fn<VType> Fn, typename LayoutType = QLayout, typename ContainerType =
        std::vector<VType *> >
    class SortedLayout {
    public:
        LayoutType *  layout;
        ContainerType container;

        SortedLayout(Fn fn, QWidget *parent = nullptr) :
        layout(new LayoutType(parent)),
        container{},
        _fn(fn),
        _parent(parent) {
        };

        void insert(VType *vtype) {
            container.push_back(vtype);
            std::ranges::sort(container, _fn);

            layout->deleteLater();
            layout = new LayoutType(_parent);
            std::ranges::for_each(container, [&](auto v) {
                layout->addWidget(v);
            });

            // *layout = std::move(*nlayout);
        }

    private:
        Fn       _fn;
        QWidget *_parent;
    };
}

#endif //X4STATIONGENERATOR_SRC_UI_UTILS_SORTEDLAYOUT_HPP
