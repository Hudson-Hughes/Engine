#include "reflection.hpp"

REFLECTION_DEFINITION(EntityID, 0, EntityID::generation, EntityID::index)
REFLECTION_DEFINITION(sister, 1, sister::a, sister::b, sister::c)
REFLECTION_DEFINITION(hambly, 2, hambly::a, hambly::b, hambly::c)
REFLECTION_DEFINITION(trevor, 3, trevor::a, trevor::b, trevor::m)
void (*ToJSONArr[4])(nlohmann::json&, void*) = { ToJSON<0>, ToJSON<1>, ToJSON<2>, ToJSON<3> };
void (*FromJSONArr[4])(nlohmann::json&, void*) = { FromJSON<0>, FromJSON<1>, FromJSON<2>, FromJSON<3> };
string ComponentNames[4] = { "EntityID", "sister", "hambly", "trevor" };
void (*ComponentDestructors[4])(void*) = { Destroy<0>, Destroy<1>, Destroy<2>, Destroy<3> };
void* (*ComponentConstructors[4])(void*) = { Construct<0>, Construct<1>, Construct<2>, Construct<3> };
void* (*ComponentCopiers[4])(void*, void*) = { Copy<0>, Copy<1>, Copy<2>, Copy<3> };
void* (*ComponentCopyScorchers[4])(void*, void*) = { CopyScorch<0>, CopyScorch<1>, CopyScorch<2>, CopyScorch<3> };
size_t ComponentSizes[4] = { sizeof(EntityID), sizeof(sister), sizeof(hambly), sizeof(trevor) };
bool ComponentParentStatus[4] = { 0, 0, 0, 0 };
int ComponentParentIndex[4] = { 0, 0, 0, 0 };
int ComponentParentList[2] = { 0, 0 };
int ParentComponentCount = 0;
