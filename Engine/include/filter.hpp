#pragma once

#include "component.hpp"

template <class T, template <class T> class U>
struct is_instance : public std::false_type {};

template <class T, template <class> class U>
struct is_instance<U<T>, U> : public std::true_type {};

template <typename KIND, typename... Ts>
struct extract_kind_components {
    static constexpr auto impl() noexcept
    {
        ComponentMask bs;
        auto append = [i = 0, &bs]<typename T>() mutable {
            if (is_instance<T, KIND>::value)
                bs.set(GetComponentIDForWrite<T>());
        };
        (append.template operator() < Ts > (), ...);
        return bs;
    }
    static constexpr ComponentMask arr = impl();
    static constexpr auto value = arr;
};

template <typename KIND, typename ...Ts>
static constexpr ComponentMask KindComponents = extract_kind_components<KIND, Ts...>::value;

struct MaskQuery {
    ComponentMask read_mask;
    ComponentMask write_mask;
    ComponentMask optional_mask;
    ComponentMask not_mask;
    ComponentMask parent_mask;
};

template <typename... Ts>
struct extrapolate_filter_types {
    static constexpr auto impl() noexcept
    {
        MaskQuery mq;
        auto append = [i = 0, &mq]<typename T>() mutable {
            if (is_instance<T, READ>::value) mq.read_mask.set(GetComponentIDForRead<T>());
            if (is_instance<T, WRITE>::value) mq.write_mask.set(GetComponentIDForWrite<T>());
            if (is_instance<T, OPTIONAL>::value) mq.optional_mask.set(GetComponentIDForOptional<T>());
            if (is_instance<T, NOT>::value) mq.not_mask.set(GetComponentIDForNot<T>());
            if (is_instance<T, PARENT>::value) mq.parent_mask.set(GetComponentIDForParent<T>());
        };
        (append.template operator() < Ts > (), ...);
        return mq;
    }
    static constexpr MaskQuery arr = impl();
    static constexpr auto value = arr;
};

template <typename ...Ts>
static constexpr MaskQuery Query = extrapolate_filter_types<Ts...>::value;