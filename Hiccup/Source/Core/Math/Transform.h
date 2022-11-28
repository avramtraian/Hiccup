// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

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
		T           Data[3 * 3];
		T           M[3][3];
		Vector3T<T> Rows[3];
	};

public:
	/**
	 * Default constructor.
	 */
	FORCEINLINE Matrix3T();

public:
	/** @return An identity matrix. */
	FORCEINLINE static Matrix3T<T> Identity();
};

using Matrix3f  = Matrix3T<float32>;
using Matrix3d  = Matrix3T<float64>;
using Matrix3u  = Matrix3T<uint32>;
using Matrix3i  = Matrix3T<int32>;

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
		T           Data[4 * 4];
		T           M[4][4];
		Vector4T<T> Rows[4];
	};

public:
	/**
	 * Default constructor.
	 */
	FORCEINLINE Matrix4T();

public:
	/** @return An identity matrix. */
	FORCEINLINE static Matrix4T<T> Identity();
};

using Matrix4f  = Matrix4T<float32>;
using Matrix4d  = Matrix4T<float64>;
using Matrix4u  = Matrix4T<uint32>;
using Matrix4i  = Matrix4T<int32>;

using Matrix4   = Matrix4f;

// Matrix4
#pragma endregion

#pragma region Matrix3 Implementation

template<typename T>
FORCEINLINE Matrix3T<T>::Matrix3T()
	: Data{}
{}

template<typename T>
FORCEINLINE Matrix3T<T> Matrix3T<T>::Identity()
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
FORCEINLINE Matrix4T<T>::Matrix4T()
	: Data{}
{}

template<typename T>
FORCEINLINE Matrix4T<T> Matrix4T<T>::Identity()
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