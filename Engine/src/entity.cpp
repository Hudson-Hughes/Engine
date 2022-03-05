#include <entity.hpp>
#include <json.hpp>
#include <bitset2.hpp>

EntityID::EntityID() {
    generation = -1;
    index = -1;
};

EntityID::EntityID(int idx, int gen) {
    index = idx;
    generation = gen;
};

bool EntityID::empty() {
    return index == -1 && generation == -1;
};

bool EntityID::operator==(const EntityID& other) {
    return other.index == index && other.generation == generation;
};

void EntityStorage::clear() {
    archetype = nullptr;
    archetypeIndex = -1;
    parent = EntityID();
    child = EntityID();
    nextSibling = EntityID();
    prevSibling = EntityID();
};

EntityStorage& EntityStore::operator[](EntityID n) {
    assert(isAlive(n));
    return entities[n.index];
}
EntityStorage& EntityStore::operator[](int n) { return entities[n]; }
EntityStorage& EntityStore::at(EntityID n) { return entities[n.index]; }

EntityID EntityStore::freshChild(EntityID parent) {
    assert(isAlive(parent));
    EntityID child = freshEntity();
    at(child).parent = parent;
    if (at(parent).child.index > -1) {
        // A Child Exists
        EntityID currentFirstChild = at(parent).child;
        at(child).nextSibling = currentFirstChild;
        at(at(parent).child).prevSibling = child;
    }
    at(parent).child = child;
    return child;
}

EntityID EntityStore::freshEntity() {
    if (!deadEntities.empty()) {
        int new_generation = entities[deadEntities.top()].generation;
        EntityID result =
            EntityID(deadEntities.top(), entities[deadEntities.top()].generation);
        at(result) = EntityStorage();
        at(result).generation = new_generation;
        at(result).clear();
        livingEntities = livingEntities + 1;
        dead.erase(deadEntities.top());
        deadEntities.pop();
        return result;
    }
    entities.push_back(EntityStorage());
    livingEntities = livingEntities + 1;
    return EntityID(((int)entities.size()) - 1, 0);
}

int EntityStore::setParent(EntityID child, EntityID parent) {
    assert(isAlive(parent) || isAlive(child));
    if (at(child).parent == parent)
        return 0;
    if (!isAlive(at(child).parent)) {
        // Has no parent
        if (at(parent).child.index > -1) {
            // A Child Exists
            EntityID currentFirstChild = at(parent).child;
            at(child).nextSibling = currentFirstChild;
            at(at(parent).child).prevSibling = child;
        }
        at(parent).child = child;
    }
    else if (isAlive(at(child).parent)) {
        // Already has a parent
        if (!isAlive(at(child).prevSibling) && isAlive(at(child).nextSibling)) {
            // Is first child
            if (!isAlive(at(child).nextSibling)) {
                // Is only child
                at(at(child).parent).child = EntityID();
            }
            else {
                // has next sibling
                at(at(child).nextSibling).prevSibling = EntityID();
                at(at(child).parent).child = at(child).nextSibling;
            }
        }
        if (isAlive(at(child).prevSibling) && isAlive(at(child).nextSibling)) {
            // Is middle child
            at(at(child).prevSibling).nextSibling = at(child).nextSibling;
            at(at(child).nextSibling).prevSibling = at(child).prevSibling;
        }
        if (isAlive(at(child).prevSibling) && !isAlive(at(child).nextSibling)) {
            // Is last child
            at(at(child).prevSibling).nextSibling = EntityID();
        }
    }
    else if (isAlive(at(parent).child)) {
        // parent already has a child
        at(at(parent).child).prevSibling = child;
        at(child).nextSibling = at(parent).child;
    }
    at(child).parent = parent;
    at(parent).child = child;
    return 0;
}

void EntityStore::orphan(EntityID child) {
    if (!isAlive(at(child).parent))
        return;
    if (!isAlive(at(child).prevSibling) && !isAlive(at(child).nextSibling)) {
        // Is only child
        at(at(child).parent).child = EntityID();
    }
    if (!isAlive(at(child).prevSibling) && isAlive(at(child).nextSibling)) {
        // Is first child
        if (!isAlive(at(child).nextSibling)) {
            // Is only child
            at(at(child).parent).child = EntityID();
        }
        else {
            // has next sibling
            at(at(child).nextSibling).prevSibling = EntityID();
            at(at(child).parent).child = at(child).nextSibling;
        }
    }
    if (isAlive(at(child).prevSibling) && isAlive(at(child).nextSibling)) {
        // Is middle child
        at(at(child).prevSibling).nextSibling = at(child).nextSibling;
        at(at(child).nextSibling).prevSibling = at(child).prevSibling;
    }
    if (isAlive(at(child).prevSibling) && !isAlive(at(child).nextSibling)) {
        // Is last child
        at(at(child).prevSibling).nextSibling = EntityID();
    }
    at(child).parent = EntityID();
    at(child).nextSibling = EntityID();
    at(child).prevSibling = EntityID();
}

bool EntityStore::isAlive(EntityID entity) {
    return entity.generation > -1 &&
        entities[entity.index].generation == entity.generation;
}

void EntityStore::killEntity(EntityID entityId) {
    if (!isAlive(entityId))
        return;
    while (isAlive(at(entityId).child)) {
        killEntity(at(entityId).child);
    }
    deadEntities.push(entityId.index);
    dead.insert(entityId.index);
    at(entityId).generation++;
    livingEntities--;
    if (!isAlive(at(entityId).parent)) {
    }
    else {
        if (!isAlive(at(entityId).prevSibling) &&
            isAlive(at(entityId).nextSibling)) {
            at(at(entityId).parent).child = at(entityId).nextSibling;
            at(at(entityId).nextSibling).prevSibling = EntityID();
        }
        if (isAlive(at(entityId).prevSibling) &&
            isAlive(at(entityId).nextSibling)) {
            at(at(entityId).prevSibling).nextSibling = at(entityId).nextSibling;
            at(at(entityId).nextSibling).prevSibling = at(entityId).prevSibling;
        }
        if (isAlive(at(entityId).prevSibling) &&
            !isAlive(at(entityId).nextSibling)) {
            at(at(entityId).prevSibling).nextSibling = EntityID();
        }
        if (!isAlive(at(entityId).prevSibling) &&
            !isAlive(at(entityId).nextSibling)) {
            at(at(entityId).parent).child = EntityID();
        }
    }
    at(entityId).parent = EntityID();
}

void EntityStore::print_out(int idx, int indent) { printTree( cout, idx, indent ); }

ostream& EntityStore::printTree(ostream& os, int idx, int indent) const {
    auto es = this;
    if (idx == -1) {
        for (int i = 0; i < this->entities.size(); i++) {
            if (!dead.count(i) && es->entities[i].parent.index == -1)
                printTree(os, i);
        }
        return os;
    }
    // os << std::string(indent, '\t') << "Idx:\t" << idx << "\tGen:\t" <<
    // es->entities[idx].generation << "\tChunk:\t" << es->entities[idx].archetype
    // << "\tIndex:\t" << es->entities[idx].archetypeIndex << std::endl;
    // if(isAlive(idx));
    os << std::string(indent, '\t') << "" << idx << "|"
        << es->entities[idx].generation << "|" << es->entities[idx].archetype
        << "|" << es->entities[idx].archetypeIndex << std::endl;
    if (es->entities[idx].child.index != -1) {
        auto currentChildIdx = es->entities[idx].child.index;
        while (currentChildIdx > -1) {
            auto childEntityStorage = es->entities[currentChildIdx];
            es->printTree(os, currentChildIdx, indent + 1);
            currentChildIdx = es->entities[currentChildIdx].nextSibling.index;
        }
    }
    return os;
}

ostream& operator<<(std::ostream& os, const EntityID& i) {
    os << i.index << "|" << i.generation;
    return os;
}
ostream& operator<<(std::ostream& os, const EntityStore& val) {
    // for(auto i = 0; i < val.entities.size(); i++){
    //     os << "Entity #" << i << "\n";
    //     os << val.entities[i];
    //     os << "\n";
    // }

    return val.printTree(os);
}
ostream& operator<<(std::ostream& os, const EntityStorage& i) {
    os << "Archetype: " << i.archetype << "\tIndex: " << i.archetypeIndex
        << "\tGen: " << i.generation << "\n"
        << "Parent: " << i.parent << "\t"
        << "Child: " << i.child << "\t"
        << "Next: " << i.nextSibling << "\t"
        << "Previous: " << i.prevSibling << "\t" << std::endl;
    return os;
}