// Copyright (c) 2022-2023 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"

#include "MathUtilities.h"
#include "Geometry.h"

namespace HC
{

#pragma region Matrix3

/**
 *----------------------------------------------------------------
 * Hiccup Matrix 3x3.
 *----------------------------------------------------------------
 */
template<typename T>
struct Matrix3T
{
public:
	union
	{
		T           data[3 * 3];
		T           m[3][3];
		Vector3T<T> rows[3];
	};

public:
	/**
	 * Default constructor.
	 */
	ALWAYS_INLINE Matrix3T();

public:
	/** @return An identity matrix. */
	ALWAYS_INLINE static Matrix3T<T> identity();
};

using Matrix3f  = Matrix3T<float32_t>;
using Matrix3d  = Matrix3T<float64_t>;
using Matrix3u  = Matrix3T<uint32_t>;
using Matrix3i  = Matrix3T<int32_t>;

using Matrix3   = Matrix3f;

// Matrix3
#pragma endregion

#pragma region Matrix4

/**
 *----------------------------------------------------------------
 * Hiccup Matrix 4x4.
 *----------------------------------------------------------------
 */
template<typename T>
struct Matrix4T
{
public:
	union
	{
		T           data[4 * 4];
		T           m[4][4];
		Vector4T<T> rows[4];
	};

public:
	/**
	 * Default constructor.
	 */
	ALWAYS_INLINE Matrix4T();

public:
	/** @return An identity matrix. */
	ALWAYS_INLINE static Matrix4T<T> identity();
};

using Matrix4f  = Matrix4T<float32_t>;
using Matrix4d  = Matrix4T<float64_t>;
using Matrix4u  = Matrix4T<uint32_t>;
using Matrix4i  = Matrix4T<int32_t>;

using Matrix4   = Matrix4f;

// Matrix4
#pragma endregion

#pragma region Matrix3 Implementation

template<typename T>
ALWAYS_INLINE Matrix3T<T>::Matrix3T()
	: data{}
{}

template<typename T>
ALWAYS_INLINE Matrix3T<T> Matrix3T<T>::identity()
{
	return Matrix3T<T>
	(
		T(1), T(0), T(0),
		T(0), T(1), T(0),
		T(0), T(0), T(1)
	);
}

// Matrix3 Implementation
#pragma endregion

#pragma region Matrix4 Implementation

template<typename T>
ALWAYS_INLINE Matrix4T<T>::Matrix4T()
	: Data{}
{}

template<typename T>
ALWAYS_INLINE Matrix4T<T> Matrix4T<T>::identity()
{
	return Matrix4T<T>
	(
		T(1), T(0), T(0), T(0),
		T(0), T(1), T(0), T(0),
		T(0), T(0), T(1), T(0),
		T(0), T(0), T(0), T(1)
	);
}

// Matrix4 Implementation
#pragma endregion

} // namespace HC