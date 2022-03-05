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
};