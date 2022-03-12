#include "world.hpp"

void World::percolateDown(EntityID entity, int newLevel) {
    Spot newSpot = Archetype::switchArchetype(getEntitySpot(entity), getArchetype(getEntityComponents(entity), newLevel));
    EntityID currentChild = es[entity].child;
    while (es.isAlive(currentChild)) {
        percolateDown(currentChild, newLevel + 1);
        bindParentPointers(currentChild, entity);
        currentChild = es[currentChild].nextSibling;
    }
};

void World::declaredirtyComponents(ComponentMask mask) {
    dirtyComponents = mask.operator|=(dirtyComponents);
}

Archetype* World::getArchetype(ComponentMask mask, int level) {
    archetypes.resize(level + 1 > archetypes.size() ? level + 1 : archetypes.size());
    auto found = std::find_if(archetypes[level].begin(), archetypes[level].end(), [&mask](const Archetype* archetype) { return archetype->mask == mask; });
    if (found == archetypes[level].end()) {
        archetypes[level].push_front(new Archetype(this, &this->es, mask, level));
        return archetypes[level].front();
    };
    return *found;
};

Spot World::getEntitySpot(EntityID entityID) {
    assert(es.isAlive(entityID));
    return { es[entityID.index].archetype, es[entityID.index].archetypeIndex };
};

EntityID World::createEntity(ComponentMask mask, EntityID parent, EntityID original) {
    EntityID result = (parent == EntityID()) ? es.freshEntity() : es.freshChild(parent);
    if (es.isAlive(original)) {
        Archetype::copyOrCreateItem(result, { es[original.index].archetype, es[original.index].archetypeIndex }, getArchetype(mask | getEntityComponents(original), getEntityLevel(parent) + 1));
    }
    else {
        Archetype::copyOrCreateItem(result, Spot(), getArchetype(mask, getEntityLevel(parent) + 1));
    }
    if (!parent.empty()) bindParentPointers(result, parent);
    return result;
};
void World::removeComponents(EntityID entity, ComponentMask toRemove) {
    ComponentMask newMask = getEntityComponents(entity);
    for (int i = 0; i < 512; i++) if (toRemove[i]) newMask.set(i, false);
    if (getEntityComponents(entity) == newMask) return;
    Spot newSpot = Archetype::switchArchetype(getEntitySpot(entity), getArchetype(newMask, getEntityLevel(entity)));
}
void World::addComponents(EntityID entity, ComponentMask toAdd) {
    ComponentMask newMask = getEntityComponents(entity);
    for (int i = 0; i < 512; i++) if (toAdd[i]) newMask.set(i, true);
    if (getEntityComponents(entity) == newMask) return;
    Spot newSpot = Archetype::switchArchetype(getEntitySpot(entity), getArchetype(newMask, getEntityLevel(entity)));
}
Archetype* World::getEntityArchetype(EntityID entityID) {
    assert(es.isAlive(entityID));
    assert(es[entityID.index].archetype);
    return es[entityID.index].archetype;
};
ComponentMask World::getEntityComponents(EntityID entityID) {
    assert(es.isAlive(entityID));
    assert(es[entityID.index].archetype);
    return es[entityID.index].archetype->mask;
};

int World::setParent(EntityID child, EntityID parent) {
    es.orphan(child); int result = es.setParent(child, parent);
    percolateDown(child, getEntityLevel(parent) + 1);
    bindParentPointers(child, parent);
    return result;
}
void World::bindParentPointers(EntityID child, EntityID parent) {
    // auto childArchetype = getEntityArchetype(child);
    // auto childIndex = es[child.index].archetypeIndex;
    // auto childTable = childArchetype->tables[childIndex / 64];
    // if (parent == EntityID()) {
    //     for (auto componentId : ComponentParentList) childTable->GetComponentParentPointerArray(componentId)[childIndex % 64] = nullptr;
    //     return;
    // }
    // auto parentArchetype = getEntityArchetype(parent);
    // auto parentIndex = es[parent.index].archetypeIndex;
    // auto parentTable = parentArchetype->tables[parentIndex / 64];
    // for (auto componentId : ComponentParentList) {
    //     cout << childTable->GetComponentParentPointerArray(componentId) << "\t" << childIndex % 64 << " gets " << parentTable->pointerGrid[parentArchetype->componentIndex[componentId]][parentIndex % 64] << endl;
    //     childTable->GetComponentParentPointerArray(componentId)[childIndex % 64] = parentTable->pointerGrid[parentArchetype->componentIndex[componentId]][parentIndex % 64];
    // }
}
Spot World::getParentSpot(EntityID child) {
    if (!es.isAlive(es[child].parent)) return {};
    return getEntitySpot(es[child].parent);
}
char* World::getParentPointer(EntityID child, int component) {
    Spot spot = getParentSpot(child);
    if (spot.archetype == nullptr) return nullptr;
    if (!spot.archetype->mask.test(component)) return nullptr;
    return (char*)spot.archetype->getPointer(component, spot.index);
}
int World::orphan(EntityID entity) {
    es.orphan(entity);
    percolateDown(entity, 0);
    bindParentPointers(entity, EntityID());
    return 0;
}
int World::kill(EntityID entity) {
    EntityID currentChild = es[entity].child;
    while (es.isAlive(currentChild)) {
        bindParentPointers(currentChild, EntityID());
        kill(currentChild);
        currentChild = es[currentChild].nextSibling;
    }
    Archetype::deleteItem(getEntitySpot(entity));
    es.killEntity(entity);
    return 0;
}
int World::getEntityLevel(EntityID entityID) {
    if (!es.isAlive(entityID)) return -1;
    assert(es[entityID.index].archetype);
    return es[entityID.index].archetype->level;
};
bool World::parentHasComponent(EntityID child, int componentID){
    if (!es.isAlive(es[child].parent)) return false;
    return es[es[child].parent].archetype->mask[componentID];
}
ostream& operator<<(ostream& os, const World& dt) {
    os << "Archetype Count: " << dt.archetypes.size() << "\tEntity Count: " << dt.es.livingEntities << endl;
    os << dt.es << endl;
    for (int i = 0; i < dt.archetypes.size(); i++) {
        for (auto const& i : dt.archetypes[i]) if(i->size) os << *i;
    }
    os << "-------------------------------";
    return os;
}