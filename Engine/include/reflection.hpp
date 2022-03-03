#pragma once

#include <json.hpp>
#include <vector>
#include <string>

using namespace std;

#define _READ const
#define _NOT volatile
#define _OPTIONAL &
#define _PARENT *

template <typename T> struct READ { using component = T; using kind = std::integral_constant<int, 0>; };
template <typename T> struct WRITTEN { using component = T; using kind = std::integral_constant<int, 1>; };
template <typename T> struct OPTIONAL { using component = T; using kind = std::integral_constant<int, 2>; };
template <typename T> struct NOT { using component = T; using kind = std::integral_constant<int, 3>; };
template <typename T> struct PARENT { using component = T; using kind = std::integral_constant<int, 4>; };

template <class T>
void ToJSON(nlohmann::json& nlohmann_json_j, void* data);
template <class T>
void ToJSON(nlohmann::json& nlohmann_json_j, T* data);
template <int T>
void ToJSON(nlohmann::json& nlohmann_json_j, void* data);

template <class T>
void FromJSON(nlohmann::json& nlohmann_json_j, void* data);
template <class T>
void FromJSON(nlohmann::json& nlohmann_json_j, T* data);
template <int T>
void FromJSON(nlohmann::json& nlohmann_json_j, void* data);

template <class T>
constexpr int GetComponentID();
template <int T>
constexpr size_t GetComponentSize();
template <class T>
void Destroy(void* ptr);
template <int T>
void Destroy(void* ptr);
template <class T>
void* Construct(void* ptr);
template <int T>
void* Construct(void* ptr);
template <class T>
void* Copy(void* src, void* dest);
template <int T>
void* Copy(void* src, void* dest);
template <class T>
void* CopyScorch(void* src, void* dest);
template <int T>
void* CopyScorch(void* src, void* dest);
template <class ...T>
constexpr int GetFilterID();
template <class ...T>
constexpr int GetParentStatus();

#define REFLECTION_DECLARATION(TYPE, INDEX, ...) \
template<> void ToJSON<TYPE>(nlohmann::json& nlohmann_json_j, void* data); \
template<> void FromJSON<TYPE>(nlohmann::json& nlohmann_json_j, void* data); \
template<> void ToJSON<TYPE>(nlohmann::json& nlohmann_json_j, TYPE* data); \
template<> void FromJSON<TYPE>(nlohmann::json& nlohmann_json_j, TYPE* data); \
template<> void ToJSON<INDEX>(nlohmann::json& nlohmann_json_j, void* data); \
template<> void FromJSON<INDEX>(nlohmann::json& nlohmann_json_j, void* data); \
template<> constexpr size_t GetComponentSize<INDEX>(){ return sizeof(TYPE); } \
template<> constexpr int GetComponentID<TYPE>(){ return INDEX; } \
template<> void Destroy<TYPE>(void* obj_ptr); \
template<> void* Construct<TYPE>(void* obj_ptr); \
template<> void Destroy<INDEX>(void* obj_ptr); \
template<> void* Construct<INDEX>(void* obj_ptr); \
template<> void* Copy<TYPE>(void* src, void* dest); \
template<> void* Copy<INDEX>(void* src, void* dest); \
template<> void* CopyScorch<TYPE>(void* src, void* dest); \
template<> void* CopyScorch<INDEX>(void* src, void* dest); \
\

#define REFLECTION_DEFINITION(TYPE, INDEX, ...) \
template<> void ToJSON<TYPE>(nlohmann::json& nlohmann_json_j, void* data) \
    { TYPE& nlohmann_json_t = *static_cast<TYPE*>(data); NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_TO, __VA_ARGS__)) }; \
template<> void FromJSON<TYPE>(nlohmann::json& nlohmann_json_j, void* data) \
    { TYPE& nlohmann_json_t = *static_cast<TYPE*>(data); NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, __VA_ARGS__)) }; \
template<> void ToJSON<TYPE>(nlohmann::json& nlohmann_json_j, TYPE* data) \
    { TYPE& nlohmann_json_t = *data; NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_TO, __VA_ARGS__)) }; \
template<> void FromJSON<TYPE>(nlohmann::json& nlohmann_json_j, TYPE* data) \
    { TYPE& nlohmann_json_t = *data; NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, __VA_ARGS__)) }; \
template<> void ToJSON<INDEX>(nlohmann::json& nlohmann_json_j, void* data) \
    { TYPE& nlohmann_json_t = *static_cast<TYPE*>(data); NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_TO, __VA_ARGS__)) }; \
template<> void FromJSON<INDEX>(nlohmann::json& nlohmann_json_j, void* data) \
    { TYPE& nlohmann_json_t = *static_cast<TYPE*>(data); NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, __VA_ARGS__)) }; \
template<> void Destroy<TYPE>(void* obj_ptr) \
    {  ((TYPE*) obj_ptr)->~TYPE(); } \
template<> void* Construct<TYPE>(void* obj_ptr) \
    { return new(obj_ptr) TYPE(); } \
template<> void Destroy<INDEX>(void* obj_ptr) \
    {  ((TYPE*) obj_ptr)->~TYPE(); } \
template<> void* Construct<INDEX>(void* obj_ptr) \
    { return new(obj_ptr) TYPE(); } \
template<> void* Copy<TYPE>(void* src, void* dest) \
    { memcpy ( dest, src, sizeof(TYPE) ); return dest; } \
template<> void* Copy<INDEX>(void* src, void* dest) \
    { memcpy ( dest, src, sizeof(TYPE) ); return dest; } \
template<> void* CopyScorch<TYPE>(void* src, void* dest) \
    { memcpy ( dest, src, sizeof(TYPE) ); Destroy<TYPE>(src); return dest; } \
template<> void* CopyScorch<INDEX>(void* src, void* dest) \
    { memcpy ( dest, src, sizeof(TYPE) ); Destroy<TYPE>(src); return dest; } \
\

#include "people.hpp"
#include "entity.hpp"
REFLECTION_DECLARATION(EntityID, 0, EntityID::generation, EntityID::index)
REFLECTION_DECLARATION(sister, 1, sister::a, sister::b, sister::c)
REFLECTION_DECLARATION(hambly, 2, hambly::a, hambly::b, hambly::c)
REFLECTION_DECLARATION(trevor, 3, trevor::a, trevor::b, trevor::m)
extern void (*ToJSONArr[4])(nlohmann::json&, void*);
extern void (*FromJSONArr[4])(nlohmann::json&, void*);
extern string ComponentNames[4];
extern void (*ComponentDestructors[4])(void*);
extern void* (*ComponentConstructors[4])(void*);
extern void* (*ComponentCopiers[4])(void*, void*);
extern void* (*ComponentCopyScorchers[4])(void*, void*);
extern size_t ComponentSizes[4];
extern bool ComponentParentStatus[4];
extern int ComponentParentIndex[4];
extern int ComponentParentList[2];
extern int ParentComponentCount;
typedef void* ParentPointerGrid[2][64];
