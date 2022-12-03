// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"

namespace HC
{

/**
 * The most common math constants.
 */
#define PI                          (3.1415926535897932F)
#define KINDA_SMALL_NUMBER          (1e-4F)
#define SMALL_NUMBER                (1e-8F)
#define BIG_NUMBER                  (3.4e38F)
#define EULUERS_NUMBER              (2.7182818284590452F)
#define GOLDEN_RATIO                (1.6180339887498948F)

#define DOUBLE_PI                   (3.1415926535897932384626433832795028)
#define DOUBLE_KINDA_SMALL_NUMBER   (1e-4)
#define DOUBLE_SMALL_NUMBER         (1e-8)
#define DOUBLE_BIG_NUMBER           (3.4e+38)
#define DOUBLE_EULUERS_NUMBER       (2.7182818284590452353602874713526624)
#define DOUBLE_GOLDEN_RATIO         (1.6180339887498948482045868343656381)

/**
 * More PI-related constants.
 */
#define INV_PI                      (0.31830988618F)
#define TWO_PI                      (6.28318530717F)
#define HALF_PI                     (1.57079632679F)
#define PI_SQUARED                  (9.86960440108F)

#define DOUBLE_INV_PI               (0.31830988618379067154)
#define DOUBLE_TWO_PI               (6.28318530717958647692)
#define DOUBLE_HALF_PI              (1.57079632679489661923)
#define DOUBLE_PI_SQUARED           (9.86960440108935861883)

/**
 * Common square roots.
 */
#define SQRT_2                      (1.41421356237F)
#define SQRT_3                      (1.73205080756F)
#define INV_SQRT_2                  (0.70710678118F)
#define INV_SQRT_3                  (0.57735026918F)
#define HALF_SQRT_2                 (0.70710678118F)
#define HALF_SQRT_3                 (0.86602540378F)

#define DOUBLE_SQRT_2               (1.4142135623730950488016887242097)
#define DOUBLE_SQRT_3               (1.7320508075688772935274463415059)
#define DOUBLE_INV_SQRT_2           (0.7071067811865475244008443621048)
#define DOUBLE_INV_SQRT_3           (0.5773502691896257645091487805019)
#define DOUBLE_HALF_SQRT_2          (0.7071067811865475244008443621048)
#define DOUBLE_HALF_SQRT_3          (0.8660254037844386467637231707529)

/**
 *----------------------------------------------------------------
 * Hiccup Math.
 *----------------------------------------------------------------
 * This structure holds the Hiccup Math Utilities core API functions.
 */
struct Math
{
public:
	// Calculates the absolute value.
	template<typename T>
	ALWAYS_INLINE static T Abs(T x)
	{
		return (x < 0) ? -x : x;
	}

	// Gets the maximum value between two numbers.
	template<typename T>
	ALWAYS_INLINE static T Max(T a, T b)
	{
		return (a > b) ? a : b;
	}

	// Gets the maximum absolute value between two numbers.
	template<typename T>
	ALWAYS_INLINE static T AbsMax(T a, T b)
	{
		return Max<T>(Abs<T>(a), Abs<T>(b));
	}

	// Gets the minimum value between two numbers.
	template<typename T>
	ALWAYS_INLINE static T Min(T a, T b)
	{
		return (a < b) ? a : b;
	}

	// Gets the minimum absolute value between two numbers.
	template<typename T>
	ALWAYS_INLINE static T AbsMin(T a, T b)
	{
		return Min<T>(Abs<T>(a), Abs<T>(b));
	}

	// Clamps a value between two numbers.
	template<typename T>
	ALWAYS_INLINE static T Clamp(T x, T min, T max)
	{
		return Min<T>(Max<T>(x, min), max);
	}

	// Whether or not the values are almost equal. This is used to dismiss floating point precision issues.
	template<typename T>
	ALWAYS_INLINE static bool AreNearlyEqual(T a, T b, T tolerance = T(KINDA_SMALL_NUMBER))
	{
		return Abs<T>(a - b) <= tolerance;
	}

public:
	// Converts from degrees to radians.
	template<typename T>
	ALWAYS_INLINE static T ToRad(T degrees)
	{
		return degrees / T(180) * T(DOUBLE_PI);
	}

	// Converts from radians to degrees.
	template<typename T>
	ALWAYS_INLINE static T ToDeg(T radians)
	{
		return radians / T(DOUBLE_PI) * T(180);
	}

public:
	/** @return The square root. */
	HC_API static float32 Sqrt(float32 x);
	HC_API static float64 Sqrt(float64 x);

	/**
	 * Calculates the sine of an angle.
	 * 
	 * @param x The angle, expressed in radians.
	 * 
	 * @return The sine of the angle.
	 */
	HC_API static float32 Sin(float32 x);
	HC_API static float64 Sin(float64 x);

	/**
	 * Calculates the cosine of an angle.
	 *
	 * @param x The angle, expressed in radians.
	 *
	 * @return The cosine of the angle.
	 */
	HC_API static float32 Cos(float32 x);
	HC_API static float64 Cos(float64 x);

	/**
	 * Calculates the tangent of an angle.
	 *
	 * @param x The angle, expressed in radians.
	 *
	 * @return The tangent of the angle.
	 */
	HC_API static float32 Tan(float32 x);
	HC_API static float64 Tan(float64 x);

	/**
	 * Calculates the arcsine of an value.
	 *
	 * @param x The value.
	 *
	 * @return The arcsine of the value, expressed in radians.
	 */
	HC_API static float32 Asin(float32 x);
	HC_API static float64 Asin(float64 x);

	/**
	 * Calculates the arccosine of an value.
	 *
	 * @param x The value.
	 *
	 * @return The arccosine of the value, expressed in radians.
	 */
	HC_API static float32 Acos(float32 x);
	HC_API static float64 Acos(float64 x);

	/**
	 * Calculates the arctangent of an value.
	 *
	 * @param x The value.
	 *
	 * @return The arctangent of the value, expressed in radians.
	 */
	HC_API static float32 Atan(float32 x);
	HC_API static float64 Atan(float64 x);
};

// Converts kilobytes to bytes.
#define Kilobytes(X) (1024ULL * X)

// Converts megabytes to bytes.
#define Megabytes(X) (1024ULL * Kilobytes(X))

// Converts gigabytes to bytes.
#define Gigabytes(X) (1024ULL * Megabytes(X))

} // namespace HC