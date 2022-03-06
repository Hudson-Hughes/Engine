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

template <class, template <class> class>
struct is_instance : public std::false_type {};

template <class T, template <class> class U>
struct is_instance<U<T>, U> : public std::true_type {};

template <typename... Ts>
struct extract_read_components {
    static constexpr auto impl() noexcept
    {
        ComponentMask bs;
        auto append = [i = 0, &bs]<typename T>() mutable {
            if (is_instance<T, READ>::value)
                bs.set(GetComponentIDForRead<T>());
        };
        (append.template operator() < Ts > (), ...);
        return bs;
    }
    static constexpr ComponentMask arr = impl();
    static constexpr auto value = arr;
};

template <typename... Ts>
struct extract_write_components {
    static constexpr auto impl() noexcept
    {
        ComponentMask bs;
        auto append = [i = 0, &bs]<typename T>() mutable {
            if (is_instance<T, WRITE>::value)
                bs.set(GetComponentIDForWrite<T>());
        };
        (append.template operator() < Ts > (), ...);
        return bs;
    }
    static constexpr ComponentMask arr = impl();
    static constexpr auto value = arr;
};

template <typename... Ts>
struct extract_optional_components {
    static constexpr auto impl() noexcept
    {
        ComponentMask bs;
        auto append = [i = 0, &bs]<typename T>() mutable {
            if (is_instance<T, OPTIONAL>::value)
                bs.set(GetComponentIDForOptional<T>());
        };
        (append.template operator() < Ts > (), ...);
        return bs;
    }
    static constexpr ComponentMask arr = impl();
    static constexpr auto value = arr;
};

template <typename... Ts>
struct extract_not_components {
    static constexpr auto impl() noexcept
    {
        ComponentMask bs;
        auto append = [i = 0, &bs]<typename T>() mutable {
            if (is_instance<T, NOT>::value)
                bs.set(GetComponentIDForNot<T>());
        };
        (append.template operator() < Ts > (), ...);
        return bs;
    }
    static constexpr ComponentMask arr = impl();
    static constexpr auto value = arr;
};

template <typename... Ts>
struct extract_parent_components {
    static constexpr auto impl() noexcept
    {
        ComponentMask bs;
        auto append = [i = 0, &bs]<typename T>() mutable {
            if (is_instance<T, PARENT>::value)
                bs.set(GetComponentIDForParent<T>());
        };
        (append.template operator() < Ts > (), ...);
        return bs;
    }
    static constexpr ComponentMask arr = impl();
    static constexpr auto value = arr;
};


template <typename ...Ts>
static constexpr ComponentMask ReadComponents = extract_read_components<Ts...>::value;
template <typename ...Ts>
static constexpr ComponentMask WriteComponents = extract_write_components<Ts...>::value;
template <typename ...Ts>
static constexpr ComponentMask OptionalComponents = extract_optional_components<Ts...>::value;
template <typename ...Ts>
static constexpr ComponentMask NotComponents = extract_not_components<Ts...>::value;
template <typename ...Ts>
static constexpr ComponentMask ParentComponents = extract_parent_components<Ts...>::value;

struct MaskQuery {
    ComponentMask read_mask;
    ComponentMask write_mask;
    ComponentMask optional_mask;
    ComponentMask not_mask;
    ComponentMask parent_mask;
};

template<typename ...Ts>
constexpr MaskQuery GenerateQueryMasks() {
    return MaskQuery(
        ReadComponents<Ts...>,
        WriteComponents<Ts...>,
        OptionalComponents<Ts...>,
        NotComponents<Ts...>,
        ParentComponents<Ts...>
    );
};

template <typename... Ts>
struct query_method {
    static constexpr auto impl() noexcept
    {
        return GenerateQueryMasks<Ts...>();
    }
    static constexpr MaskQuery arr = impl();
    static constexpr auto value = arr;
};

template <typename ...Ts>
static constexpr MaskQuery Query = query_method<Ts...>::value;