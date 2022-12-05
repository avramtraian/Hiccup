// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include <cstdint>

namespace HC
{

using uint8     = uint8_t;
using uint16    = uint16_t;
using uint32    = uint32_t;
using uint64    = uint64_t;

using int8      = int8_t;
using int16     = int16_t;
using int32     = int32_t;
using int64     = int64_t;

using float32   = float;
using float64   = double;

using bool8     = int8;
using bool32    = int32;

using usize     = size_t;
using uptr      = uintptr_t;

// The type cannot be copied.
#define HC_NON_COPIABLE(TYPE_NAME)          \
	TYPE_NAME(const TYPE_NAME&) = delete;   \
	TYPE_NAME& operator=(const TYPE_NAME&) = delete;

// The type cannot be moved.
#define HC_NON_MOVABLE(TYPE_NAME)                           \
	TYPE_NAME(TYPE_NAME&&) noexcept = delete;               \
	TYPE_NAME& operator=(TYPE_NAME&&) noexcept = delete;

/**
 *----------------------------------------------------------------
 * Hiccup Types Utilities.
 *----------------------------------------------------------------
*/
struct Types
{
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

	template<typename T, usize N>
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

} // namespace HC