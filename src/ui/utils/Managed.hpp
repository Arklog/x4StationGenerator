//
// Created by pierre on 7/27/26.
//

#ifndef X4STATIONGENERATOR_SRC_UI_UTILS_MANAGED_HPP
#define X4STATIONGENERATOR_SRC_UI_UTILS_MANAGED_HPP
#include <type_traits>

namespace ui::utils {
    template<typename T, typename V>
    concept ManagedCallback = requires(T t, V v)
    {
        { t(v) };
        std::is_reference_v<V>;
    };

    template<typename T, ManagedCallback<T> callback, bool call_if_modified = true>
    struct Managed {
        using value_type = T;

        Managed(T &v, callback fn) :
        value(v),
        fn{fn} {
            if constexpr (call_if_modified)
                former = v;
        }

        ~Managed() {
            if constexpr (call_if_modified) {
                if (value != former) {
                    fn(value);
                }
            } else {
                fn(value);
            }
        }

        value_type &get() {
            return value;
        }

        Managed &operator=(const value_type &v) {
            value = v;
            return *this;
        }

        Managed &operator=(value_type &&v) {
            value = v;
            return *this;
        }

        value_type *operator->() {
            return &value;
        }

        operator value_type &() {
            return value;
        }

        value_type &value;

    private:
        callback   fn;
        value_type former;
    };
}

#endif //X4STATIONGENERATOR_SRC_UI_UTILS_MANAGED_HPP
