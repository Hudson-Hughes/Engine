#pragma once

#include <cstdio>
#include <cinttypes>
#include <vector>
#include <queue>
#include <map>
#include <iostream>
#include <unordered_set>
#include <stack>
#include <assert.h>

using namespace std;

class Archetype;

///Component
class EntityID {
public:
    int generation;
    int index;
    EntityID();
    EntityID(int idx, int gen);
    bool empty();
    bool operator==(const EntityID& other);
    friend ostream& operator<<(ostream& os, const EntityID& dt);
};
std::ostream& operator<<(std::ostream& os, const EntityID& i);

struct EntityStorage {
    int generation = 0;
    Archetype* archetype = nullptr;
    int archetypeIndex = -1;
    EntityID parent = EntityID();
    EntityID child = EntityID();
    EntityID nextSibling = EntityID();
    EntityID prevSibling = EntityID();
    void clear();

    friend ostream& operator<<(ostream& os, const EntityStorage& dt);
};
std::ostream& operator<<(std::ostream& os, const EntityStorage& i);

class EntityStore {
    friend struct EntityStorage;

private:
    std::stack<int> deadEntities;

public:
    int livingEntities = 0;
    std::unordered_set<int> dead;
    std::vector<EntityStorage> entities;
    EntityStorage& operator[](EntityID n);
    EntityStorage& operator[](int n);
    EntityStorage& at(EntityID n);
    EntityID freshEntity();
    EntityID freshChild(EntityID parent);

    int setParent(EntityID child, EntityID parent);
    void orphan(EntityID child);
    bool isAlive(EntityID entity);
    void killEntity(EntityID entityId);
    ostream& printTree(ostream& os, int idx = 0, int indent = 0) const;

    void print_out(int idx, int indent);

    friend ostream& operator<<(ostream& os, const EntityStore& dt);
};
std::ostream& operator<<(std::ostream& os, const EntityStore& i);