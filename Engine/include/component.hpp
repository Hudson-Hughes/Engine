#pragma once

#include <bitset2.hpp>
#include <sstream>
#include <type_traits>

#include "reflection.hpp"

using ComponentMask = Bitset2::bitset2<512>;
typedef int ComponentIndexes[512];
typedef size_t OffsetMap[512];
typedef ComponentMask QueryMask[5];

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


template <typename ...Ts>
static constexpr ComponentMask ComponentsMask = component_mask_from_class<Ts...>::value;
std::string ComponentMaskAsString(ComponentMask mask);

ComponentMask AlteredMask(ComponentMask add = ComponentMask(0), ComponentMask remove = ComponentMask(0), ComponentMask original = ComponentMask(0));