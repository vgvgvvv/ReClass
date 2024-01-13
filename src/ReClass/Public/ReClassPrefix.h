#pragma once
#include "ReClass_API.h"
#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <utility>
#include <functional>
#include <memory>
#include <cassert>
#include <unordered_map>

#define RECLASS_API ReClass_API

#define RE_CLASS(...)
#define RE_FIELD(...)
#define RE_ENUM(...)
#define RE_FUNCTION(...)

#define RECLASS_MOVE(V) std::move(V)
#define RECLASS_CHECK(CONDITION) assert(CONDITION)

#define RECLASS_STATIC_CLASS_FUNCNAME StaticClass

namespace ReClassSystem
{
    class Type;
    class Class;

#define DEFINE_NUMBER_TYPE(NUMTYPE) \
using NUMTYPE = NUMTYPE##_t;

    DEFINE_NUMBER_TYPE(int8)
    DEFINE_NUMBER_TYPE(int16)
    DEFINE_NUMBER_TYPE(int32)
    DEFINE_NUMBER_TYPE(int64)
    DEFINE_NUMBER_TYPE(uint8)
    DEFINE_NUMBER_TYPE(uint16)
    DEFINE_NUMBER_TYPE(uint32)
    DEFINE_NUMBER_TYPE(uint64)

#undef DEFINE_NUMBER_TYPE

    template<typename Item1, typename Item2>
    using Pair = std::pair<Item1, Item2>;
#define RECLASS_MAKE_PAIR std::make_pair
#define RECLASS_GET_FROM_PAIR(PAIR, COUNT) std::get<COUNT>(PAIR)

    using String = std::string;
    template<typename TFunc>
    using Function = std::function<TFunc>;
    template<typename T>
    using Vector = std::vector<T>;
    template<typename K, typename V>
    using Map = std::map<K, V>;
    template<typename K, typename V>
    using UnorderedMap = std::unordered_map<K, V>;

    template<typename T>
    using SharedPtr = std::shared_ptr<T>;
#define RECLASS_MAKE_SHARED(T) std::make_shared<T>
#define RECLASS_SHAREDPTR_STATIC_CAST std::static_pointer_cast
    template<typename T>
    using UniquePtr = std::unique_ptr<T>;
#define RECLASS_MAKE_UNIQUE(T) std::make_unique<T>

}


#define RECLASS_NO_DISCARD [[nodiscard]]