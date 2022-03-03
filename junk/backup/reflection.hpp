#pragma once

#include "json.hpp"

#include "entity.hpp"
#include "people.hpp"

#define _READ const
#define _NOT volatile
#define _OPTIONAL &
#define _PARENT *

using namespace std;

template <typename T> struct READ { using component = T; using kind = std::integral_constant<int, 0>; };
template <typename T> struct WRITTEN { using component = T; using kind = std::integral_constant<int, 1>; };
template <typename T> struct OPTIONAL { using component = T; using kind = std::integral_constant<int, 2>; };
template <typename T> struct NOT { using component = T; using kind = std::integral_constant<int, 3>; };
template <typename T> struct PARENT { using component = T; using kind = std::integral_constant<int, 4>; };

template<class T>
void ToJSON(nlohmann::json& nlohmann_json_j, void* data);
template<class T>
void FromJSON(nlohmann::json& nlohmann_json_j, void* data);
template<class T>
void ToJSON(nlohmann::json& nlohmann_json_j, T* data);
template<class T>
void FromJSON(nlohmann::json& nlohmann_json_j, T* data);
template<int T>
void ToJSON(nlohmann::json& nlohmann_json_j, void* data);
template<int T>
void FromJSON(nlohmann::json& nlohmann_json_j, void* data);
template<class T>
constexpr int GetComponentID();
template<int T>
constexpr size_t GetComponentSize();
template<class T>
void Destroy(void* ptr);
template<int T>
void Destroy(void* ptr);
template<class T>
void* Construct(void* ptr);
template<int T>
void* Construct(void* ptr);
template<class T>
void* Copy(void* src, void* dest);
template<int T>
void* Copy(void* src, void* dest);
template<class T>
void* CopyScorch(void* src, void* dest);
template<int T>
void* CopyScorch(void* src, void* dest);
template<class ...T>
constexpr int GetFilterID();
template<class ...T>
constexpr int GetParentStatus();

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EntityID, EntityID::generation, EntityID::index);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(sister, sister::a, sister::b, sister::c);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(hambly, hambly::a, hambly::b, hambly::c);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(trevor, trevor::a, trevor::b, trevor::h, trevor::m);
template<> void ToJSON<EntityID>(nlohmann::json& nlohmann_json_j, void* data);
template<> void ToJSON<sister>(nlohmann::json& nlohmann_json_j, void* data);
template<> void ToJSON<hambly>(nlohmann::json& nlohmann_json_j, void* data);
template<> void ToJSON<trevor>(nlohmann::json& nlohmann_json_j, void* data);
template<> void FromJSON<EntityID>(nlohmann::json& nlohmann_json_j, void* data);
template<> void FromJSON<sister>(nlohmann::json& nlohmann_json_j, void* data);
template<> void FromJSON<hambly>(nlohmann::json& nlohmann_json_j, void* data);
template<> void FromJSON<trevor>(nlohmann::json& nlohmann_json_j, void* data);
template<> void ToJSON<EntityID>(nlohmann::json& nlohmann_json_j, EntityID* data);
template<> void ToJSON<sister>(nlohmann::json& nlohmann_json_j, sister* data);
template<> void ToJSON<hambly>(nlohmann::json& nlohmann_json_j, hambly* data);
template<> void ToJSON<trevor>(nlohmann::json& nlohmann_json_j, trevor* data);
template<> void FromJSON<EntityID>(nlohmann::json& nlohmann_json_j, EntityID* data);
template<> void FromJSON<sister>(nlohmann::json& nlohmann_json_j, sister* data);
template<> void FromJSON<hambly>(nlohmann::json& nlohmann_json_j, hambly* data);
template<> void FromJSON<trevor>(nlohmann::json& nlohmann_json_j, trevor* data);
template<> void ToJSON<0>(nlohmann::json& nlohmann_json_j, void* data);
template<> void ToJSON<1>(nlohmann::json& nlohmann_json_j, void* data);
template<> void ToJSON<2>(nlohmann::json& nlohmann_json_j, void* data);
template<> void ToJSON<3>(nlohmann::json& nlohmann_json_j, void* data);
template<> void FromJSON<0>(nlohmann::json& nlohmann_json_j, void* data);
template<> void FromJSON<1>(nlohmann::json& nlohmann_json_j, void* data);
template<> void FromJSON<2>(nlohmann::json& nlohmann_json_j, void* data);
template<> void FromJSON<3>(nlohmann::json& nlohmann_json_j, void* data);
template<> constexpr int GetComponentID<EntityID>() { return 0; };
template<> constexpr int GetComponentID<sister>() { return 1; };
template<> constexpr int GetComponentID<hambly>() { return 2; };
template<> constexpr int GetComponentID<trevor>() { return 3; };
template<> constexpr size_t GetComponentSize<0>();
template<> constexpr size_t GetComponentSize<1>();
template<> constexpr size_t GetComponentSize<2>();
template<> constexpr size_t GetComponentSize<3>();
template<> void Destroy<EntityID>(void* obj_ptr);
template<> void Destroy<sister>(void* obj_ptr);
template<> void Destroy<hambly>(void* obj_ptr);
template<> void Destroy<trevor>(void* obj_ptr);
template<> void Destroy<0>(void* obj_ptr);
template<> void Destroy<1>(void* obj_ptr);
template<> void Destroy<2>(void* obj_ptr);
template<> void Destroy<3>(void* obj_ptr);
template<> void* Construct<EntityID>(void* obj_ptr);
template<> void* Construct<sister>(void* obj_ptr);
template<> void* Construct<hambly>(void* obj_ptr);
template<> void* Construct<trevor>(void* obj_ptr);
template<> void* Construct<0>(void* obj_ptr);
template<> void* Construct<1>(void* obj_ptr);
template<> void* Construct<2>(void* obj_ptr);
template<> void* Construct<3>(void* obj_ptr);
template<> void* Copy<EntityID>(void* src, void* dest);
template<> void* Copy<sister>(void* src, void* dest);
template<> void* Copy<hambly>(void* src, void* dest);
template<> void* Copy<trevor>(void* src, void* dest);
template<> void* Copy<0>(void* src, void* dest);
template<> void* Copy<1>(void* src, void* dest);
template<> void* Copy<2>(void* src, void* dest);
template<> void* Copy<3>(void* src, void* dest);
template<> void* CopyScorch<EntityID>(void* src, void* dest);
template<> void* CopyScorch<sister>(void* src, void* dest);
template<> void* CopyScorch<hambly>(void* src, void* dest);
template<> void* CopyScorch<trevor>(void* src, void* dest);
template<> void* CopyScorch<0>(void* src, void* dest);
template<> void* CopyScorch<1>(void* src, void* dest);
template<> void* CopyScorch<2>(void* src, void* dest);
template<> void* CopyScorch<3>(void* src, void* dest);

void (*ToJSONArr[4])(nlohmann::json&, void*);
void (*FromJSONArr[4])(nlohmann::json&, void*);
string ComponentNames[4];
void (*ComponentDestructors[4])(void*);
void* (*ComponentConstructors[4])(void*);
void* (*ComponentCopiers[4])(void*, void*);
void* (*ComponentCopyScorchers[4])(void*, void*);
size_t ComponentSizes[4];
bool ComponentParentStatus[4];
int ComponentParentIndex[4];
int ComponentParentList[2];
int ParentComponentCount;
typedef void* ParentPointerGrid[2][64];

template<> constexpr int GetComponentID<int>() { return -1; };

template<> constexpr int GetFilterID<READ<sister>, WRITTEN<hambly>>() { return 1; };
template<> constexpr int GetFilterID<WRITTEN<hambly>, READ<sister>>() { return 1; };
template<> constexpr int GetFilterID<READ<trevor>, WRITTEN<hambly>, PARENT<sister>, PARENT<EntityID>>() { return 2; };

template<> constexpr int GetParentStatus<trevor>() { return 0; };
template<> constexpr int GetParentStatus<EntityID>() { return 1; };
template<> constexpr int GetParentStatus<hambly>() { return 0; };
template<> constexpr int GetParentStatus<sister>() { return 1; };

constexpr int FilterCount() { return 3; }