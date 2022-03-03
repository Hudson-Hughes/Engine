#include "table.hpp"

Table::~Table() {
    alloc.deallocate(buffer, owner->table_size);
}
Table::Table(Archetype* owner) {
    buffer = alloc.allocate(owner->table_size);
    auto parentComponentCount = 0;
    for (auto i = 0; i < owner->components.size(); i++) {
        pointerGrid.push_back(vector<void*>(64));
    }
    for (int i = 0; i < ParentComponentCount; i++) for (int j = 0; j < 64; j++) parentPointerGrid[i][j] = NULL;
}
inline bool Table::full() { return count == 64; };
void* Table::GetComponentArray(int componentID) {
    return buffer + owner->offsetMap[componentID];
}
void** Table::GetComponentParentPointerArray(int componentID) {
    assert(ComponentParentStatus[componentID]);
    // return NULL;
    return parentPointerGrid[ComponentParentIndex[componentID]];
}