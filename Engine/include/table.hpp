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
        return (T*) GetComponentArray(GetComponentID<T>());
    };
    void* GetComponentArray(int componentID);

    template <typename T>
    T& GetComponent(int index) {
        return GetComponentArray<T>()[index];
    };

    template<typename T>
    bool HasComponent() {
        return HasComponent(GetComponentID<T>());
    }
    bool HasComponent(int componentID);

    template<typename T>
    struct Iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;

        Iterator(pointer ptr) : m_ptr(ptr) {}

        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }
        Iterator<T>& operator++() { m_ptr++; return *this; }  
        Iterator<T> operator++(int) { Iterator<T> tmp = *this; ++(*this); return tmp; }
        friend bool operator== (const Iterator<T>& a, const Iterator<T>& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const Iterator<T>& a, const Iterator<T>& b) { return a.m_ptr != b.m_ptr; };  

    private:
        pointer m_ptr;
    };

    template<typename T>
    Iterator<T> begin() { assert(HasComponent<T>()); return Iterator<T>(&GetComponentArray<T>()[0]); }
    template<typename T>
    Iterator<T> end()   { assert(HasComponent<T>()); return Iterator<T>(&GetComponentArray<T>()[count]); }

    template<typename T>
    struct ConstIterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;

        ConstIterator(pointer ptr) : m_ptr(ptr) {}

        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }
        const ConstIterator<T>& operator++() { m_ptr++; return *this; }  
        const ConstIterator<T> operator++(int) { Iterator<T> tmp = *this; ++(*this); return tmp; }
        friend bool operator== (const ConstIterator<T>& a, const ConstIterator<T>& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const ConstIterator<T>& a, const ConstIterator<T>& b) { return a.m_ptr != b.m_ptr; };  

    private:
        pointer m_ptr;
    };

    template<typename T>
    ConstIterator<T> cbegin() { assert(HasComponent<T>()); return ConstIterator<T>(&GetComponentArray<T>()[0]); }
    template<typename T>
    ConstIterator<T> cend()   { assert(HasComponent<T>()); return ConstIterator<T>(&GetComponentArray<T>()[count]); }

    template<typename T>
    T& operator[] (int index)
    {
        assert(HasComponent<T>() && index < count);
        return &GetComponentArray<T>()[index];
    }

    template<typename T>
    const T& operator[] (int index)
    {
        assert(HasComponent<T>() && index < count);
        return &GetComponentArray<T>()[index];
    }
};