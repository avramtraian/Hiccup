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
	HC_API static float32 Float32();

	/**
	 * Generates a random 32-bit floating point number in the given range [min, max).
	 * 
	 * @param min The lower bound of the range.
	 * @param max The upper bound of the range.
	 * 
	 * @return The random generated value.
	 */
	ALWAYS_INLINE static float32 Float32Range(float32 min, float32 max)
	{
		return min + (max - min) * Float32();
	}

	/** @return A random 64-bit floating point number, in the range [0, 1). */
	HC_API static float64 Float64();

	/**
	 * Generates a random 64-bit floating point number in the given range [min, max).
	 * 
	 * @param min The lower bound of the range.
	 * @param max The upper bound of the range.
	 * 
	 * @return The random generated value.
	 */
	ALWAYS_INLINE static float64 Float64Range(float64 min, float64 max)
	{
		return min + (max - min) * Float64();
	}

	/** @return A random 32-bit unsigned integer. */
	HC_API static uint32 UInt32();

	/**
	 * Generates a random 32-bit unsigned integer value in the given range [min, max].
	 * 
	 * @param min The lower bound of the range.
	 * @param max The upper bound of the range.
	 *
	 * @return The random generated value.
	 */
	ALWAYS_INLINE static uint32 UInt32Range(uint32 min, uint32 max)
	{
		return min + UInt32() % (max - min + 1);
	}

	/** @return A random 64-bit unsigned integer. */
	HC_API static uint64 UInt64();

	/**
	 * Generates a random 64-bit unsigned integer value in the given range [min, max].
	 *
	 * @param min The lower bound of the range.
	 * @param max The upper bound of the range.
	 *
	 * @return The random generated value.
	 */
	ALWAYS_INLINE static uint64 UInt64Range(uint64 min, uint64 max)
	{
		return min + UInt64() % (max - min + 1);
	}

	/** @return A random 32-bit signed integer. */
	ALWAYS_INLINE static int32 Int32()
	{
		const uint32 v = UInt32();
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
	ALWAYS_INLINE static int32 Int32Range(int32 min, int32 max)
	{
		return min + UInt32() % (max - min + 1);
	}

	/** @return A random 32-bit signed integer. */
	ALWAYS_INLINE static int64 Int64()
	{
		const uint64 v = UInt64();
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
	ALWAYS_INLINE static int64 Int64Range(int64 min, int64 max)
	{
		return min + UInt64() % (max - min + 1);
	}
};

} // namespace HC