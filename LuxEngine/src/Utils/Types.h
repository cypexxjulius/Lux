#pragma once 

#include <glm/glm.hpp>
#include <memory>
#include <array>
#include <unordered_map>

namespace Lux
{

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T, int count>
using Array = std::array<T, count>;

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint32_t;
using u8 = uint8_t;

using byte = uint8_t;

using i64 = int64_t;
using i32 = int32_t;
using i16 = int32_t;
using i8 = int8_t;

using f32 = float;

using mat4 = glm::mat4;

using v2 = glm::vec2;

using v3 = glm::vec3;

using v4 = glm::vec4;

enum class DataType : u32
{
    INVALID,
    FLOAT,
    INT,
    BYTE,
    CHAR,
    BOOL,
    V2,
    V3,
    V4,
    MAT2,
    MAT3,
    MAT4,
    SAMPLER1D,
    SAMPLER2D,
    SAMPLER3D,
};

}