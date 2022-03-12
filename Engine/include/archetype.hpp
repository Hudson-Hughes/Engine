#pragma once

#include <set>
#include <iostream>
#include <sstream>
#include <vector>
#include <utility>
#include <set>
#include <iostream>
#include <list>
#include <sstream>

#include <json.hpp>
#include "component.hpp"
#include "entity.hpp"
#include "table.hpp"
#include "spot.hpp"

using namespace std;

class World;
class Table;

class Archetype {
	int allocateItem(EntityID entityID, bool initialize);
	void deallocateItem(int index, bool destruct);
	void updateEntityStorage(EntityID entityID, Spot spot);

public:
	ComponentMask mask;
	set<int64_t> dynamic_tags;
	int level = 0;
	int size = 0;
	EntityStore* es;
	World* owner;
	ComponentMask dirty;
	OffsetMap offsetMap;
	size_t table_size;
	vector<Table*> tables;
	ComponentIndexes componentIndex;
	vector<int> components;

	Archetype(World* owner, EntityStore* es, ComponentMask _mask, int level);

	inline int getLastIndex();

	inline int getNextIndex();

	void* getPointer(int component, int index) const;

	EntityID getEntityID(int index);

	void setupOffsetMap();

	static Spot switchArchetype(Spot source, Archetype* destinationArchetype);

	template<typename T>
	T* GetComponentArray(int tableIndex) {
		return GetComponentArray(tableIndex, GetComponentID<T>());
	};

	void* GetComponentArray(int tableIndex, int componentID);

	static void deleteItem(Spot source);

	static Spot copyOrCreateItem(EntityID entityID, Spot source, Archetype* destinationArchetype);

	friend ostream& operator<<(ostream& os, const Archetype& val);

	using table_iterator = std::vector<Table*>::const_iterator;

    table_iterator begin() const { return tables.begin(); }
    table_iterator end() const { return tables.end(); }

	template<typename T>
    bool HasComponent() {
        return HasComponent(GetComponentID<T>());
    }
    bool HasComponent(int componentID);

    // template<typename T>
    // struct ComponentIterator
    // {
    //     using iterator_category = std::forward_iterator_tag;
    //     using difference_type   = std::ptrdiff_t;
    //     using value_type        = T;
    //     using pointer           = T*;
    //     using reference         = T&;

    //     ComponentIterator(pointer ptr) : m_ptr(ptr) {}

    //     reference operator*() const { return *m_ptr; }
    //     pointer operator->() { return m_ptr; }
    //     ComponentIterator<T>& operator++() { m_ptr++; return *this; }  
    //     ComponentIterator<T> operator++(int) { ComponentIterator<T> tmp = *this; ++(*this); return tmp; }
    //     friend bool operator== (const ComponentIterator<T>& a, const ComponentIterator<T>& b) { return a.m_ptr == b.m_ptr; };
    //     friend bool operator!= (const ComponentIterator<T>& a, const ComponentIterator<T>& b) { return a.m_ptr != b.m_ptr; };  

    // private:
    //     pointer m_ptr;
    // };

    // template<typename T>
    // ComponentIterator<T> begin() { assert(HasComponent<T>()); return ComponentIterator<T>(&GetComponentArray<T>()[0]); }
    // template<typename T>
    // ComponentIterator<T> end()   { assert(HasComponent<T>()); return ComponentIterator<T>(&GetComponentArray<T>()[count]); }

    // template<typename T>
    // struct ConstComponentIterator
    // {
    //     using iterator_category = std::forward_iterator_tag;
    //     using difference_type   = std::ptrdiff_t;
    //     using value_type        = T;
    //     using pointer           = T*;
    //     using reference         = T&;

    //     ConstComponentIterator(pointer ptr) : m_ptr(ptr) {}

    //     reference operator*() const { return *m_ptr; }
    //     pointer operator->() { return m_ptr; }
    //     const ConstComponentIterator<T>& operator++() { m_ptr++; return *this; }  
    //     const ConstComponentIterator<T> operator++(int) { Iterator<T> tmp = *this; ++(*this); return tmp; }
    //     friend bool operator== (const ConstComponentIterator<T>& a, const ConstComponentIterator<T>& b) { return a.m_ptr == b.m_ptr; };
    //     friend bool operator!= (const ConstComponentIterator<T>& a, const ConstComponentIterator<T>& b) { return a.m_ptr != b.m_ptr; };  

    // private:
    //     pointer m_ptr;
    // };

    // template<typename T>
    // ConstComponentIterator<T> cbegin() { assert(HasComponent<T>()); return ConstComponentIterator<T>(&GetComponentArray<T>()[0]); }
    // template<typename T>
    // ConstComponentIterator<T> cend()   { assert(HasComponent<T>()); return ConstComponentIterator<T>(&GetComponentArray<T>()[count]); }

};