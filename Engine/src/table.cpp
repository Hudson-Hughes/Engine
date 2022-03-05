#include "table.hpp"

Table::~Table() {
    alloc.deallocate(buffer, owner->table_size);
}
Table::Table(Archetype* owner) {
    buffer = alloc.allocate(owner->table_size);
    
}
inline bool Table::full() { return count == 64; };
void* Table::GetComponentArray(int componentID) {
    return buffer + owner->offsetMap[componentID];
}
