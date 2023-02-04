// Copyright (c) 2022-2023 Avram Traian. All rights reserved.

#pragma once

#include <cstdint>

// The type cannot be copied.
#define HC_NON_COPIABLE(TYPE_NAME)          \
	TYPE_NAME(const TYPE_NAME&) = delete;   \
	TYPE_NAME& operator=(const TYPE_NAME&) = delete;

// The type cannot be moved.
#define HC_NON_MOVABLE(TYPE_NAME)                           \
	TYPE_NAME(TYPE_NAME&&) noexcept = delete;               \
	TYPE_NAME& operator=(TYPE_NAME&&) noexcept = delete;

namespace HC
{

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

} // namespace HC