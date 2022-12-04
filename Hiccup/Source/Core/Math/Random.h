// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

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
	HC_API static float32 float_32();

	/**
	 * Generates a random 32-bit floating point number in the given range [min, max).
	 * 
	 * @param min The lower bound of the range.
	 * @param max The upper bound of the range.
	 * 
	 * @return The random generated value.
	 */
	ALWAYS_INLINE static float32 float_32_range(float32 min, float32 max)
	{
		return min + (max - min) * float_32();
	}

	/** @return A random 64-bit floating point number, in the range [0, 1). */
	HC_API static float64 float_64();

	/**
	 * Generates a random 64-bit floating point number in the given range [min, max).
	 * 
	 * @param min The lower bound of the range.
	 * @param max The upper bound of the range.
	 * 
	 * @return The random generated value.
	 */
	ALWAYS_INLINE static float64 float_64_range(float64 min, float64 max)
	{
		return min + (max - min) * float_64();
	}

	/** @return A random 32-bit unsigned integer. */
	HC_API static uint32 uint_32();

	/**
	 * Generates a random 32-bit unsigned integer value in the given range [min, max].
	 * 
	 * @param min The lower bound of the range.
	 * @param max The upper bound of the range.
	 *
	 * @return The random generated value.
	 */
	ALWAYS_INLINE static uint32 uint_32_range(uint32 min, uint32 max)
	{
		return min + uint_32() % (max - min + 1);
	}

	/** @return A random 64-bit unsigned integer. */
	HC_API static uint64 uint_64();

	/**
	 * Generates a random 64-bit unsigned integer value in the given range [min, max].
	 *
	 * @param min The lower bound of the range.
	 * @param max The upper bound of the range.
	 *
	 * @return The random generated value.
	 */
	ALWAYS_INLINE static uint64 uint64_range(uint64 min, uint64 max)
	{
		return min + uint_64() % (max - min + 1);
	}

	/** @return A random 32-bit signed integer. */
	ALWAYS_INLINE static int32 int_32()
	{
		const uint32 v = uint_32();
		return *(const int32*)(&v);
	}

	/**
	 * Generates a random 32-bit signed integer value in the given range [min, max].
	 *
	 * @param min The lower bound of the range.
	 * @param max The upper bound of the range.
	 *
	 * @return The random generated value.
	 */
	ALWAYS_INLINE static int32 int32_range(int32 min, int32 max)
	{
		return min + uint_32() % (max - min + 1);
	}

	/** @return A random 32-bit signed integer. */
	ALWAYS_INLINE static int64 int_64()
	{
		const uint64 v = uint_64();
		return *(const int64*)(&v);
	}

	/**
	 * Generates a random 64-bit signed integer value in the given range [min, max].
	 *
	 * @param min The lower bound of the range.
	 * @param max The upper bound of the range.
	 *
	 * @return The random generated value.
	 */
	ALWAYS_INLINE static int64 int_64_range(int64 min, int64 max)
	{
		return min + uint_64() % (max - min + 1);
	}
};

} // namespace HC