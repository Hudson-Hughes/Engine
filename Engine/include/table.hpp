#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <assert.h>

#include <bitset2.hpp>

#include "component.hpp"
#include "archetype.hpp"

using namespace std;

class Archetype;

class Table {
public:
    int count = 0;
    Archetype* owner;
    std::allocator<char> alloc;
    char* buffer;
    ~Table();
    Table(Archetype* owner);
    inline bool full();;
    template <typename T>
    T* GetComponentArray() {
        return GetComponentArray(GetComponentID<T>());
    };
    void* GetComponentArray(int componentID);
    
};