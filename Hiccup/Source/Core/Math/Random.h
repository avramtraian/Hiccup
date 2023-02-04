// Copyright (c) 2022-2023 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"

#include "Geometry.h"

namespace HC
{

/**
 *----------------------------------------------------------------
 * Hiccup Random.
 *---------------------------------------------------------------- 
 */
struct Random
{
public:
	/** @return A random 32-bit floating point number, in the range [0, 1). */
	HC_API static float32_t float_32();

	/**
	 * Generates a random 32-bit floating point number in the given range [min, max).
	 * 
	 * @param min The lower bound of the range.
	 * @param max The upper bound of the range.
	 * 
	 * @return The random generated value.
	 */
	ALWAYS_INLINE static float32_t float_32_range(float32_t min, float32_t max)
	{
		return min + (max - min) * float_32();
	}

	/** @return A random 64-bit floating point number, in the range [0, 1). */
	HC_API static float64_t float_64();

	/**
	 * Generates a random 64-bit floating point number in the given range [min, max).
	 * 
	 * @param min The lower bound of the range.
	 * @param max The upper bound of the range.
	 * 
	 * @return The random generated value.
	 */
	ALWAYS_INLINE static float64_t float_64_range(float64_t min, float64_t max)
	{
		return min + (max - min) * float_64();
	}

	/** @return A random 32-bit unsigned integer. */
	HC_API static uint32_t uint_32();

	/**
	 * Generates a random 32-bit unsigned integer value in the given range [min, max].
	 * 
	 * @param min The lower bound of the range.
	 * @param max The upper bound of the range.
	 *
	 * @return The random generated value.
	 */
	ALWAYS_INLINE static uint32_t uint_32_range(uint32_t min, uint32_t max)
	{
		return min + uint_32() % (max - min + 1);
	}

	/** @return A random 64-bit unsigned integer. */
	HC_API static uint64_t uint_64();

	/**
	 * Generates a random 64-bit unsigned integer value in the given range [min, max].
	 *
	 * @param min The lower bound of the range.
	 * @param max The upper bound of the range.
	 *
	 * @return The random generated value.
	 */
	ALWAYS_INLINE static uint64_t uint64_t_range(uint64_t min, uint64_t max)
	{
		return min + uint_64() % (max - min + 1);
	}

	/** @return A random 32-bit signed integer. */
	ALWAYS_INLINE static int32_t int_32()
	{
		const uint32_t v = uint_32();
		return *(const int32_t*)(&v);
	}

	/**
	 * Generates a random 32-bit signed integer value in the given range [min, max].
	 *
	 * @param min The lower bound of the range.
	 * @param max The upper bound of the range.
	 *
	 * @return The random generated value.
	 */
	ALWAYS_INLINE static int32_t int32_range(int32_t min, int32_t max)
	{
		return min + uint_32() % (max - min + 1);
	}

	/** @return A random 32-bit signed integer. */
	ALWAYS_INLINE static int64_t int_64()
	{
		const uint64_t v = uint_64();
		return *(const int64_t*)(&v);
	}

	/**
	 * Generates a random 64-bit signed integer value in the given range [min, max].
	 *
	 * @param min The lower bound of the range.
	 * @param max The upper bound of the range.
	 *
	 * @return The random generated value.
	 */
	ALWAYS_INLINE static int64_t int_64_range(int64_t min, int64_t max)
	{
		return min + uint_64() % (max - min + 1);
	}
};

} // namespace HC