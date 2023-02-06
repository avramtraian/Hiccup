// Copyright (c) 2022-2023 Avram Traian. All rights reserved.

#pragma once

#include <cstdint>

// The type cannot be copied.
#define HC_NON_COPIABLE(TYPE_NAME)          \
    TYPE_NAME(const TYPE_NAME&) = delete;   \
    TYPE_NAME& operator=(const TYPE_NAME&) = delete;

// The type cannot be moved.
#define HC_NON_MOVABLE(TYPE_NAME)               \
    TYPE_NAME(TYPE_NAME&&) noexcept = delete;   \
    TYPE_NAME& operator=(TYPE_NAME&&) noexcept = delete;

namespace HC
{

static constexpr size_t InvalidSize = (size_t)(-1);

using float32_t = float;
using float64_t = double;

using ReadonlyByte = const uint8_t;
using WriteonlyByte = uint8_t;
using ReadWriteByte = uint8_t;

using ReadonlyBytes = ReadonlyByte*;
using WriteonlyBytes = WriteonlyByte*;
using ReadWriteBytes = ReadWriteByte*;

/**
 *----------------------------------------------------------------
 * Hiccup Types Utilities.
 *----------------------------------------------------------------
*/
struct Types
{
public:
    HC_NON_COPIABLE(Types);
    HC_NON_MOVABLE(Types);

    Types() = delete;

public:
    template<typename T>
    struct RemoveReference
    {
        using Type = T;
    };

    template<typename T>
    struct RemoveReference<T&>
    {
        using Type = T;
    };

    template<typename T>
    struct RemoveReference<T&&>
    {
        using Type = T;
    };

    template<typename T>
    using RemoveReferenceType = typename RemoveReference<T>::Type;

    template<typename T>
    struct RemoveConst
    {
        using Type = T;
    };

    template<typename T>
    struct RemoveConst<const T>
    {
        using Type = T;
    };

    template<typename T>
    using RemoveConstType = typename RemoveConst<T>::Type;

    template<typename T>
    struct ArrayToPointerDecay
    {
        using Type = T;
    };

    template<typename T, size_t N>
    struct ArrayToPointerDecay<T[N]>
    {
        using Type = T*;
    };

    template<typename T>
    using ArrayToPointerDecayType = typename ArrayToPointerDecay<T>::Type;

public:
    template<typename T>
    ALWAYS_INLINE static constexpr RemoveReferenceType<T>&& move(T&& object) noexcept
    {
        return static_cast<RemoveReferenceType<T>&&>(object);
    }

    template<typename T>
    ALWAYS_INLINE static constexpr T&& forward(RemoveReferenceType<T>& object) noexcept
    {
        return static_cast<T&&>(object);
    }
};

// All hashes are 64-bit unsigned integers.
using Hash = uint64_t;

template<typename T>
struct TypeTraits
{
public:
    HC_NON_COPIABLE(TypeTraits);
    HC_NON_MOVABLE(TypeTraits);

    TypeTraits() = delete;

public:
    ALWAYS_INLINE static Hash hash(const T& instance)
    {
        return Hash(1);
    }

    ALWAYS_INLINE static bool equal(const T& a, const T& b)
    {
        return (a == b);
    }
};

} // namespace HC