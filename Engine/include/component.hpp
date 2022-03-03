#pragma once

#include <bitset2.hpp>
#include <sstream>

#include "reflection.hpp"

using ComponentMask = Bitset2::bitset2<512>;
typedef int ComponentIndexes[512];
typedef size_t OffsetMap[512];
typedef ComponentMask MaskQuery[4];

template <typename... Ts>
struct component_mask_from_class {
    static constexpr auto impl() noexcept
    {
        ComponentMask bs;
        auto append = [i = 0, &bs]<typename T>() mutable {
            if (!std::is_const<T>::value && !std::is_volatile<T>::value && !std::is_reference<T>::value)
                bs.set(GetComponentID<T>());
        };
        (append.template operator() < Ts > (), ...);
        return bs;
    }
    static constexpr ComponentMask arr = impl();
    static constexpr auto value = arr;
};

template <typename... Ts>
struct component_mask_from_const_class {
    static constexpr auto impl() noexcept
    {
        ComponentMask bs;
        auto append = [i = 0, &bs]<typename T>() mutable {
            if (std::is_const<T>::value)
                bs.set(GetComponentID<typename std::remove_const<T>::type>());
        };
        (append.template operator() < Ts > (), ...);
        return bs;
    }
    static constexpr ComponentMask arr = impl();
    static constexpr auto value = arr;
};

template <typename... Ts>
struct component_mask_from_volatile_class {
    static constexpr auto impl() noexcept
    {
        ComponentMask bs;
        auto append = [i = 0, &bs]<typename T>() mutable {
            if (std::is_volatile<T>::value)
                bs.set(GetComponentID<typename std::remove_volatile<T>::type>());
        };
        (append.template operator() < Ts > (), ...);
        return bs;
    }
    static constexpr ComponentMask arr = impl();
    static constexpr auto value = arr;
};

template <typename... Ts>
struct component_mask_from_reference_class {
    static constexpr auto impl() noexcept
    {
        ComponentMask bs;
        auto append = [i = 0, &bs]<typename T>() mutable {
            if (std::is_reference<T>::value)
                bs.set(GetComponentID<typename std::remove_reference<T>::type>());
        };
        (append.template operator() < Ts > (), ...);
        return bs;
    }
    static constexpr ComponentMask arr = impl();
    static constexpr auto value = arr;
};
template <typename ...Ts>
static constexpr ComponentMask ComponentsMask = component_mask_from_class<Ts...>::value;

template <typename ...Ts>
static constexpr ComponentMask ComponentMaskFromClass = component_mask_from_class<Ts...>::value;

template <typename ...Ts>
static constexpr ComponentMask ComponentMaskFromConstClass = component_mask_from_const_class<Ts...>::value;

template <typename ...Ts>
static constexpr ComponentMask ComponentMaskFromVolatileClass = component_mask_from_volatile_class<Ts...>::value;

template <typename ...Ts>
static constexpr ComponentMask ComponentMaskFromReferenceClass = component_mask_from_reference_class<Ts...>::value;

std::string ComponentMaskAsString(ComponentMask mask);

ComponentMask AlteredMask(ComponentMask add = ComponentMask(0), ComponentMask remove = ComponentMask(0), ComponentMask original = ComponentMask(0));