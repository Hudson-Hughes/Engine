#pragma once

#include <list>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <queue>
#include <ostream>

#include "archetype.hpp"
#include "table.hpp"
#include "component.hpp"
#include "entity.hpp"
#include "spot.hpp"

using namespace std;

class World {
    friend class Archetype;
    friend class Filter;
    //friend class FilterResults;
    friend class Query;
    friend class Iterator;

    void percolateDown(EntityID entity, int newLevel);

public:
    EntityStore es;
    vector<list<Archetype*>> archetypes;
    ComponentMask dirtyComponents = ComponentMask();
    set<int64_t> dirtyTags = set<int64_t>();
    void declaredirtyComponents(ComponentMask mask);
    Archetype* getArchetype(ComponentMask mask, int level = 0);
    Spot getEntitySpot(EntityID entityID);
    EntityID createEntity(ComponentMask mask = ComponentMask(0), EntityID parent = EntityID(), EntityID original = EntityID());
    template <typename T>
    T* getParentPointer(EntityID child) {
        return getParentPointer(child, GetComponentID<T>());
    }
    void removeComponents(EntityID entity, ComponentMask toRemove);
    void addComponents(EntityID entity, ComponentMask toAdd);
    Archetype* getEntityArchetype(EntityID entityID);
    ComponentMask getEntityComponents(EntityID entityID);
    int setParent(EntityID child, EntityID parent);
    void bindParentPointers(EntityID child, EntityID parent);
    Spot getParentSpot(EntityID child);
    char* getParentPointer(EntityID child, int component);
    int orphan(EntityID entity);
    int kill(EntityID entity);
    int getEntityLevel(EntityID entityID);

    friend ostream& operator<<(ostream& os, const World& dt);

    //FilterResults filterResults[512];
    char* systems[512];
};



ostream& operator<<(ostream& os, const World& dt);