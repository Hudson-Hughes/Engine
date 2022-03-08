#pragma once

#include <tuple>
#include <type_traits>
#include "component.hpp"
#include "reflection.hpp"
#include "archetype.hpp"
#include "filter.hpp"

using namespace std;

enum SystemGroup { placeholder, physics, logic, graphics };

class ArchetypeMatchStore {
    public:
    ArchetypeMatchStore(){

    }
    // template< SystemGroup G = SystemGroup::placeholder, int priority = 0, typename ...Ts >
    // friend class System;
};

class Sys {
    public:
    MaskQuery query;
    virtual bool cycle(float delta = 1.0f) = 0;
    Sys() {
        cout << "Sys Constructor" << endl;
    }
};

template<typename Target, typename ListHead, typename... ListTails>
constexpr size_t getTypeIndexInTemplateList()
{
    if constexpr (std::is_same<Target, ListHead>::value)
        return 0;
    else
        return 1 + getTypeIndexInTemplateList<Target, ListTails...>();
}

template < typename T >
class Iter { 
    public:
    Iter() {};
    Iter(int i) { data = i; };
    int data = 3;
};

template< SystemGroup G = SystemGroup::placeholder, int priority = 0, typename ...Ts >
class System : public Sys {
    public:
    static vector<Archetype*> matches;
    static constexpr SystemGroup group = G;
    static constexpr int orderPriority = priority;
    using Item = tuple<typename Ts::component...>;
    using Itera = tuple<Iter<typename Ts::component>...>;
    Item items;
    Itera itera;
    System() : Sys() {
        query = Query<Ts...>;
        cout << "System Constructor" << endl;
        itera = make_tuple(Iter<Ts::component>(16)...);
        cout << get<0>(itera).data << endl;
    }

    virtual bool forEach(float delta, EntityID entityID, Ts::as_argument...){
        cout << "System forEach" << endl;
        return false;
    }

    bool cycle(float delta = 1.0f) override {
        cout << "System cycle" << endl;
        forEach(delta, EntityID(), get<getTypeIndexInTemplateList<Ts, Ts...>()>(items)...);
        return true;
    }
};

class HamblySystem : public System<placeholder, 4, WRITE<hambly>, PARENT<sister>> {
    public:
    bool forEach(float delta, EntityID entityID, hambly& ham, const sister& sis) override {
        cout << "HamblySystem forEach" << endl;
        return true;
    }
};

// class SisterSystem : System<READ<sister>> {
//     public:
//     // bool forEach(float delta, EntityID entityID, sister sis) override {
//     //     cout << "SisterSystem forEach" << endl;
//     //     return true;
//     // }
// };