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
#include <variant>

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

    template <typename T>
    T& getComponent(int index) const {
        assert(mask[GetComponentID<T>]);
        int idx = index % 64;
        return tables[index / 64].GetComponentArray<T>()[idx];
    }

    template<typename T>
    T* getPointer(int index){
        return getPointer(GetComponentID<T>(), index);
    }

	void* getPointer(int component, int index) const;

    template<int K_id>
    void* getPointerWithKind(int component, int index);

    template<>
    void* getPointerWithKind<2>(int component, int index){
        return mask[component] ? getPointer(component, index) : nullptr;
    }

    template<>
    void* getPointerWithKind<3>(int component, int index){
        return mask[component] ? getPointer(component, index) : nullptr;
    }

    template<>
    void* getPointerWithKind<5>(int component, int index){
        return getParentPointer(component, index);
    }

    template<>
    void* getPointerWithKind<6>(int component, int index){
        return getParentPointer(component, index);
    }

    template<typename T>
    T* getParentPointer(int index){
        return getParentPointer(GetComponentID<T>(), index);
    }

    void* getParentPointer(int component, int index);

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

    template< typename K, typename T = K::component >
    variant<T*, T> grab(){
        if(K::kind::value > 1)
        return new T();
        else
        return T();
    }

    // template<template<typename> class K, typename T>
    // template< int K, typename T >
    // class Iterator;

    // template< typename T >
    // class Iterator<0, T>{
        
    // };

    // template< typename T >
    // class Iterator<1, T>{

    // };



};