// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"

#include "MathUtilities.h"

namespace HC
{

#pragma region Forward Declarations

template<typename T>
struct Vector2T;

template<typename T>
struct Vector3T;

template<typename T>
struct Vector4T;

template<typename T>
struct RayT;

template<typename T>
struct AABB2T;

template<typename T>
struct AABB3T;

// Forward Declarations
#pragma endregion

#pragma region Vector2

/**
 *----------------------------------------------------------------
 * Hiccup 2-Component Vector.
 *----------------------------------------------------------------
 */
template<typename T>
struct Vector2T
{
public:
	// The vector's component on the X-axis.
	T X;

	// The vector's component on the Y-axis.
	T Y;

	/**
	 * Default constructor.
	 */
	ALWAYS_INLINE Vector2T();

	/**
	 * Copy constructor.
	 *
	 * @param other The vector to copy.
	 */
	ALWAYS_INLINE Vector2T(const Vector2T<T>& other);

	/**
	 * Component constructor.
	 *
	 * @param x The component on the X-axis.
	 * @param y The component on the Y-axis.
	 */
	ALWAYS_INLINE Vector2T(T x, T y);

	/**
	 * Scalar constructor. Initializes all components with the given scalar.
	 *
	 * @param scalar The value to initialize the components with.
	 */
	ALWAYS_INLINE explicit Vector2T(T scalar);

	/**
	 * Constructor using a 3-component vector.
	 * 
	 * @param vector3 The 3-component vector.
	 */
	ALWAYS_INLINE explicit Vector2T(const Vector3T<T>& vector3);

	/**
	 * Constructor using a 4-component vector.
	 * 
	 * @param vector4 The 4-component vector.
	 */
	ALWAYS_INLINE explicit Vector2T(const Vector4T<T>& vector4);

	/**
	 * Copy assignment operator.
	 *
	 * @param other The vector to copy.
	 *
	 * @return Reference to this, after the copy.
	 */
	ALWAYS_INLINE Vector2T<T>& operator=(const Vector2T<T>& other);

public:
	/**
	 * Addition operator. Adds a vector to this.
	 *
	 * @param other The vector to add.
	 *
	 * @return The vector created by adding a vector to this.
	 */
	ALWAYS_INLINE Vector2T<T> operator+(const Vector2T<T>& other) const;

	/**
	 * Addition operator. Adds a vector to this.
	 *
	 * @param other The vector to add.
	 *
	 * @return Reference to this, after the addition.
	 */
	ALWAYS_INLINE Vector2T<T>& operator+=(const Vector2T<T>& other);

	/**
	 * Subtraction operator. Subtracts a vector from this.
	 *
	 * @param other The vector to subtract.
	 *
	 * @return The vector created by subtracting a vector from this.
	 */
	ALWAYS_INLINE Vector2T<T> operator-(const Vector2T<T>& other) const;

	/**
	 * Subtraction operator. Subtracts a vector from this.
	 *
	 * @param other The vector to subtract.
	 *
	 * @return Reference to this, after the subtraction.
	 */
	ALWAYS_INLINE Vector2T<T>& operator-=(const Vector2T<T>& other);

	/**
	 * Multiplication operator. Multiplies this with a scalar.
	 *
	 * @param scalar The scalar to multiply with.
	 *
	 * @return The vector created by multiplying this with the given scalar.
	 */
	ALWAYS_INLINE Vector2T<T> operator*(T scalar) const;

	/**
	 * Multiplication operator. Multiplies this with a scalar.
	 *
	 * @param scalar The scalar to multiply with.
	 *
	 * @return Reference to this, after the multiplication.
	 */
	ALWAYS_INLINE Vector2T<T>& operator*=(T scalar);

	/**
	 * Division operator. Divides this by a scalar.
	 *
	 * @param scalar The scalar to divide with.
	 *
	 * @return The vector created by dividing this by the given scalar.
	 */
	ALWAYS_INLINE Vector2T<T> operator/(T scalar) const;

	/**
	 * Division operator. Divides this by a scalar.
	 *
	 * @param scalar The scalar to divide with.
	 *
	 * @return Reference to this, after the division.
	 */
	ALWAYS_INLINE Vector2T<T>& operator/=(T scalar);

public:
	/** @return The squared magnitude of the vector. */
	ALWAYS_INLINE T MagnitudeSquared() const;

	/** @return The magnitude of the vector. */
	ALWAYS_INLINE T Magnitude() const;

	/**
	 * Normalizes this vector.
	 *
	 * @return The vector's normal.
	 */
	ALWAYS_INLINE Vector2T<T> Normalize() const;

	/**
	 * Normalizes this vector, the calculation being made only if the vector is not already normalized.
	 *
	 * @param tolerance The maximum allowed absolute difference between the vector's squared magnitude and 1
	 *   for which the vector is considered already normalized.
	 *
	 * @return The vector's normal.
	 */
	ALWAYS_INLINE Vector2T<T> NormalizeIf(T tolerance = T(KINDA_SMALL_NUMBER)) const;

	/**
	 * Normalizes this vector, only if it is safe to do it (its magnitude is not 0).
	 *
	 * @param errorResult The value to return when the normal doesn't exist.
	 * @param threshold The minimum allowed value for the vector's squared magnitude for which the vector's
	 *   normal exists.
	 *
	 * @return The vector's normal (if possible) or the specified result for error.
	 */
	ALWAYS_INLINE Vector2T<T> NormalizeSafe(const Vector2T<T>& errorResult, T threshold = T(KINDA_SMALL_NUMBER)) const;

	/**
	 * Normalizes this vector, only if it is safe to do it (its magnitude is not 0) and the vector isn't already
	 *   normalized.
	 *
	 * @param errorResult The value to return when the normal doesn't exist.
	 * @param threshold The minimum allowed value for the vector's squared magnitude for which the vector's
	 *   normal exists.
	 * @param tolerance The maximum allowed absolute difference between the vector's squared magnitude and 1
	 *   for which the vector is considered already normalized.
	 *
	 * @return The vector's normal (if possible) or the specified result for error.
	 */
	ALWAYS_INLINE Vector2T<T> NormalizeSafeIf(const Vector2T<T>& errorResult, T threshold = T(KINDA_SMALL_NUMBER), T tolerance = T(KINDA_SMALL_NUMBER)) const;

	/**
	 * Calculates the dot product between two vectors.
	 *
	 * @param a The first vector.
	 * @param b The second vector.
	 *
	 * @return The dot product.
	 */
	ALWAYS_INLINE static T Dot(const Vector2T<T>& a, const Vector2T<T>& b);

public:
	/**
	 * Reflects a vector based on a normal.
	 *
	 * @param direction The direction to reflect.
	 * @param normal The normal to reflect against.
	 *
	 * @return The reflected direction.
	 */
	ALWAYS_INLINE static Vector2T<T> Reflect(const Vector2T<T>& direction, const Vector2T<T>& normal);
};

// 2-Component vector, with 32-bit floating point precision.
using Vector2f  = Vector2T<float32>;

// 2-Component vector, with 64-bit floating point precision.
using Vector2d  = Vector2T<float64>;

// 2-Component vector, with 32-bit unsigned integer precision.
using Vector2u  = Vector2T<uint32>;

// 2-Component vector, with 32-bit signed integer precision.
using Vector2i  = Vector2T<int32>;

// 2-Component vector, with floating point precision.
using Vector2   = Vector2f;

// Vector2
#pragma endregion

#pragma region Vector3

/**
 *----------------------------------------------------------------
 * Hiccup 3-Component Vector.
 *----------------------------------------------------------------
 */
template<typename T>
struct Vector3T
{
public:
	// The vector's component on the X-axis.
	T X;

	// The vector's component on the Y-axis.
	T Y;

	// The vector's component on the Z-axis.
	T Z;

public:
	/**
	 * Default constructor.
	 */
	ALWAYS_INLINE Vector3T();

	/**
	 * Copy constructor.
	 * 
	 * @param other The vector to copy.
	 */
	ALWAYS_INLINE Vector3T(const Vector3T<T>& other);

	/**
	 * Component constructor.
	 * 
	 * @param x The component on the X-axis.
	 * @param y The component on the Y-axis.
	 * @param z The component on the Z-axis.
	 */
	ALWAYS_INLINE Vector3T(T x, T y, T z);

	/**
	 * Scalar constructor. Initializes all components with the given scalar.
	 * 
	 * @param scalar The value to initialize the components with.
	 */
	ALWAYS_INLINE explicit Vector3T(T scalar);

	/**
	 * Constructor using a 2-component vector, for initializing the X and Y components.
	 *   The Z component is initialized with 0.
	 * 
	 * @param vector2 The 2-component vector.
	 */
	ALWAYS_INLINE explicit Vector3T(const Vector2T<T>& vector2);

	/**
	 * Constructor using a 2-component vector, for initializing the X and Y components.
	 *   The Z component is initialized by a separate given value.
	 *
	 * @param vector2 The 2-component vector.
	 * @param z The value to initialize the vector's Z component with.
	 */
	ALWAYS_INLINE Vector3T(const Vector2T<T>& vector2, T z);
	
	/**
	 * Constructor using a 4-component vector.
	 * 
	 * @param vector4 The 4-component vector.
	 */
	ALWAYS_INLINE explicit Vector3T(const Vector4T<T>& vector4);

	/**
	 * Copy assignment operator.
	 * 
	 * @param other The vector to copy.
	 * 
	 * @return Reference to this, after the copy.
	 */
	ALWAYS_INLINE Vector3T<T>& operator=(const Vector3T<T>& other);

public:
	/**
	 * Addition operator. Adds a vector to this.
	 * 
	 * @param other The vector to add.
	 * 
	 * @return The vector created by adding a vector to this.
	 */
	ALWAYS_INLINE Vector3T<T> operator+(const Vector3T<T>& other) const;

	/**
	 * Addition operator. Adds a vector to this.
	 *
	 * @param other The vector to add.
	 *
	 * @return Reference to this, after the addition.
	 */
	ALWAYS_INLINE Vector3T<T>& operator+=(const Vector3T<T>& other);

	/**
	 * Subtraction operator. Subtracts a vector from this.
	 *
	 * @param other The vector to subtract.
	 *
	 * @return The vector created by subtracting a vector from this.
	 */
	ALWAYS_INLINE Vector3T<T> operator-(const Vector3T<T>& other) const;

	/**
	 * Subtraction operator. Subtracts a vector from this.
	 *
	 * @param other The vector to subtract.
	 *
	 * @return Reference to this, after the subtraction.
	 */
	ALWAYS_INLINE Vector3T<T>& operator-=(const Vector3T<T>& other);

	/**
	 * Multiplication operator. Multiplies this with a scalar.
	 * 
	 * @param scalar The scalar to multiply with.
	 * 
	 * @return The vector created by multiplying this with the given scalar.
	 */
	ALWAYS_INLINE Vector3T<T> operator*(T scalar) const;

	/**
	 * Multiplication operator. Multiplies this with a scalar.
	 *
	 * @param scalar The scalar to multiply with.
	 *
	 * @return Reference to this, after the multiplication.
	 */
	ALWAYS_INLINE Vector3T<T>& operator*=(T scalar);

	/**
	 * Division operator. Divides this by a scalar.
	 *
	 * @param scalar The scalar to divide with.
	 *
	 * @return The vector created by dividing this by the given scalar.
	 */
	ALWAYS_INLINE Vector3T<T> operator/(T scalar) const;

	/**
	 * Division operator. Divides this by a scalar.
	 *
	 * @param scalar The scalar to divide with.
	 *
	 * @return Reference to this, after the division.
	 */
	ALWAYS_INLINE Vector3T<T>& operator/=(T scalar);

public:
	/** @return The squared magnitude of the vector. */
	ALWAYS_INLINE T MagnitudeSquared() const;

	/** @return The magnitude of the vector. */
	ALWAYS_INLINE T Magnitude() const;

	/**
	 * Normalizes this vector.
	 * 
	 * @return The vector's normal.
	 */
	ALWAYS_INLINE Vector3T<T> Normalize() const;

	/**
	 * Normalizes this vector, the calculation being made only if the vector is not already normalized.
	 * 
	 * @param tolerance The maximum allowed absolute difference between the vector's squared magnitude and 1
	 *   for which the vector is considered already normalized.
	 * 
	 * @return The vector's normal.
	 */
	ALWAYS_INLINE Vector3T<T> NormalizeIf(T tolerance = T(KINDA_SMALL_NUMBER)) const;

	/**
	 * Normalizes this vector, only if it is safe to do it (its magnitude is not 0).
	 * 
	 * @param errorResult The value to return when the normal doesn't exist.
	 * @param threshold The minimum allowed value for the vector's squared magnitude for which the vector's
	 *   normal exists.
	 * 
	 * @return The vector's normal (if possible) or the specified result for error.
	 */
	ALWAYS_INLINE Vector3T<T> NormalizeSafe(const Vector3T<T>& errorResult, T threshold = T(KINDA_SMALL_NUMBER)) const;

	/**
	 * Normalizes this vector, only if it is safe to do it (its magnitude is not 0) and the vector isn't already
	 *   normalized.
	 * 
	 * @param errorResult The value to return when the normal doesn't exist.
	 * @param threshold The minimum allowed value for the vector's squared magnitude for which the vector's
	 *   normal exists.
	 * @param tolerance The maximum allowed absolute difference between the vector's squared magnitude and 1
	 *   for which the vector is considered already normalized.
	 * 
	 * @return The vector's normal (if possible) or the specified result for error.
	 */
	ALWAYS_INLINE Vector3T<T> NormalizeSafeIf(const Vector3T<T>& errorResult, T threshold = T(KINDA_SMALL_NUMBER), T tolerance = T(KINDA_SMALL_NUMBER)) const;

	/**
	 * Calculates the dot product between two vectors.
	 * 
	 * @param a The first vector.
	 * @param b The second vector.
	 * 
	 * @return The dot product.
	 */
	ALWAYS_INLINE static T Dot(const Vector3T<T>& a, const Vector3T<T>& b);

	/**
	 * Calculates the cross product between two vectors.
	 * 
	 * @param a The first vector.
	 * @param b The second vector.
	 * 
	 * @return The cross product.
	 */
	ALWAYS_INLINE static Vector3T<T> Cross(const Vector3T<T>& a, const Vector3T<T>& b);

public:
	/**
	 * Reflects a vector based on a normal.
	 * 
	 * @param direction The direction to reflect.
	 * @param normal The normal to reflect against.
	 * 
	 * @return The reflected direction.
	 */
	ALWAYS_INLINE static Vector3T<T> Reflect(const Vector3T<T>& direction, const Vector3T<T>& normal);
};

// 3-Component vector, with 32-bit floating point precision.
using Vector3f  = Vector3T<float32>;

// 3-Component vector, with 64-bit floating point precision.
using Vector3d  = Vector3T<float64>;

// 3-Component vector, with 32-bit unsigned integer precision.
using Vector3u  = Vector3T<uint32>;

// 3-Component vector, with 32-bit signed integer precision.
using Vector3i  = Vector3T<int32>;

// 3-Component vector, with floating point precision.
using Vector3   = Vector3f;

// Vector3
#pragma endregion

#pragma region Vector4

/**
 *----------------------------------------------------------------
 * Hiccup 4-Component Vector.
 *----------------------------------------------------------------
 */
template<typename T>
struct Vector4T
{
public:
	// The vector's component on the X-axis.
	T X;

	// The vector's component on the Y-axis.
	T Y;

	// The vector's component on the Z-axis.
	T Z;

	// The vector's component on the W-axis.
	T W;

public:
	/**
	 * Default constructor.
	 */
	ALWAYS_INLINE Vector4T();

	/**
	 * Copy constructor.
	 *
	 * @param other The vector to copy.
	 */
	ALWAYS_INLINE Vector4T(const Vector4T<T>& other);

	/**
	 * Component constructor.
	 *
	 * @param x The component on the X-axis.
	 * @param y The component on the Y-axis.
	 * @param z The component on the Z-axis.
	 * @param w The component on the W-axis.
	 */
	ALWAYS_INLINE Vector4T(T x, T y, T z, T w);

	/**
	 * Scalar constructor. Initializes all components with the given scalar.
	 *
	 * @param scalar The value to initialize the components with.
	 */
	ALWAYS_INLINE explicit Vector4T(T scalar);

	/**
	 * Constructor using a 2-component vector, for the X and Y components. The
	 *   Z and W components are initialized with 0.
	 * 
	 * @param vector2 The 2-component vector.
	 */
	ALWAYS_INLINE explicit Vector4T(const Vector2T<T>& vector2);

	/**
	 * Constructor using a 2-component vector, for the X and Y components. The
	 *   Z and W components are initialized with separate, given values.
	 *
	 * @param vector2 The 2-component vector.
	 * @param z The value to initialize the vector's Z component with.
	 * @param w The value to initialize the vector's W component with.
	 */
	ALWAYS_INLINE Vector4T(const Vector2T<T>& vector2, T z, T w);

	/**
	 * Constructor using a 3-component vector, for the X, Y and Z components. The
	 *   W component is initialized with 0.
	 *
	 * @param vector3 The 3-component vector.
	 */
	ALWAYS_INLINE explicit Vector4T(const Vector3T<T>& vector3);

	/**
	 * Constructor using a 3-component vector, for the X, Y and Z components. The
	 *   W component is initialized with a separate, given value.
	 *
	 * @param vector3 The 3-component vector.
	 * @param w The value to initialize the vector's W component with.
	 */
	ALWAYS_INLINE Vector4T(const Vector3T<T>& vector3, T w);

	/**
	 * Copy assignment operator.
	 *
	 * @param other The vector to copy.
	 *
	 * @return Reference to this, after the copy.
	 */
	ALWAYS_INLINE Vector4T<T>& operator=(const Vector4T<T>& other);

public:
	/**
	 * Addition operator. Adds a vector to this.
	 *
	 * @param other The vector to add.
	 *
	 * @return The vector created by adding a vector to this.
	 */
	ALWAYS_INLINE Vector4T<T> operator+(const Vector4T<T>& other) const;

	/**
	 * Addition operator. Adds a vector to this.
	 *
	 * @param other The vector to add.
	 *
	 * @return Reference to this, after the addition.
	 */
	ALWAYS_INLINE Vector4T<T>& operator+=(const Vector4T<T>& other);

	/**
	 * Subtraction operator. Subtracts a vector from this.
	 *
	 * @param other The vector to subtract.
	 *
	 * @return The vector created by subtracting a vector from this.
	 */
	ALWAYS_INLINE Vector4T<T> operator-(const Vector4T<T>& other) const;

	/**
	 * Subtraction operator. Subtracts a vector from this.
	 *
	 * @param other The vector to subtract.
	 *
	 * @return Reference to this, after the subtraction.
	 */
	ALWAYS_INLINE Vector4T<T>& operator-=(const Vector4T<T>& other);

	/**
	 * Multiplication operator. Multiplies this with a scalar.
	 *
	 * @param scalar The scalar to multiply with.
	 *
	 * @return The vector created by multiplying this with the given scalar.
	 */
	ALWAYS_INLINE Vector4T<T> operator*(T scalar) const;

	/**
	 * Multiplication operator. Multiplies this with a scalar.
	 *
	 * @param scalar The scalar to multiply with.
	 *
	 * @return Reference to this, after the multiplication.
	 */
	ALWAYS_INLINE Vector4T<T>& operator*=(T scalar);

	/**
	 * Division operator. Divides this by a scalar.
	 *
	 * @param scalar The scalar to divide with.
	 *
	 * @return The vector created by dividing this by the given scalar.
	 */
	ALWAYS_INLINE Vector4T<T> operator/(T scalar) const;

	/**
	 * Division operator. Divides this by a scalar.
	 *
	 * @param scalar The scalar to divide with.
	 *
	 * @return Reference to this, after the division.
	 */
	ALWAYS_INLINE Vector4T<T>& operator/=(T scalar);
};

// 4-Component vector, with 32-bit floating point precision.
using Vector4f  = Vector4T<float32>;

// 4-Component vector, with 64-bit floating point precision.
using Vector4d  = Vector4T<float64>;

// 4-Component vector, with 32-bit unsigned integer precision.
using Vector4u  = Vector4T<uint32>;

// 4-Component vector, with 32-bit signed integer precision.
using Vector4i  = Vector4T<int32>;

// 4-Component vector, with floating point precision.
using Vector4   = Vector4f;

// Vector4
#pragma endregion

#pragma region Vector2 Implementation

//////////////// VECTOR2 IMPLEMENTATION ////////////////

template<typename T>
ALWAYS_INLINE Vector2T<T>::Vector2T()
	: X(T(0))
	, Y(T(0))
{}

template<typename T>
ALWAYS_INLINE Vector2T<T>::Vector2T(const Vector2T<T>& other)
	: X(other.X)
	, Y(other.Y)
{}

template<typename T>
ALWAYS_INLINE Vector2T<T>::Vector2T(T x, T y)
	: X(x)
	, Y(y)
{}

template<typename T>
ALWAYS_INLINE Vector2T<T>::Vector2T(T scalar)
	: X(scalar)
	, Y(scalar)
{}

template<typename T>
ALWAYS_INLINE Vector2T<T>::Vector2T(const Vector3T<T>& vector3)
	: X(vector3.X)
	, Y(vector3.Y)
{}

template<typename T>
ALWAYS_INLINE Vector2T<T>::Vector2T(const Vector4T<T>& vector4)
	: X(vector4.X)
	, Y(vector4.Y)
{}

template<typename T>
ALWAYS_INLINE Vector2T<T>& Vector2T<T>::operator=(const Vector2T<T>& other)
{
	X = other.X;
	Y = other.Y;
	return *this;
}

template<typename T>
ALWAYS_INLINE Vector2T<T> Vector2T<T>::operator+(const Vector2T<T>& other) const
{
	return Vector2T<T>(X + other.X, Y + other.Y);
}

template<typename T>
ALWAYS_INLINE Vector2T<T>& Vector2T<T>::operator+=(const Vector2T<T>& other)
{
	X += other.X;
	Y += other.Y;
	return *this;
}

template<typename T>
ALWAYS_INLINE Vector2T<T> Vector2T<T>::operator-(const Vector2T<T>& other) const
{
	return Vector2T<T>(X - other.X, Y - other.Y);
}

template<typename T>
ALWAYS_INLINE Vector2T<T>& Vector2T<T>::operator-=(const Vector2T<T>& other)
{
	X -= other.X;
	Y -= other.Y;
	return *this;
}

template<typename T>
ALWAYS_INLINE Vector2T<T> Vector2T<T>::operator*(T scalar) const
{
	return Vector2T<T>(X * scalar, Y * scalar);
}

template<typename T>
ALWAYS_INLINE Vector2T<T> operator*(T scalar, const Vector2T<T>& vector)
{
	return vector * scalar;
}

template<typename T>
ALWAYS_INLINE Vector2T<T>& Vector2T<T>::operator*=(T scalar)
{
	X *= scalar;
	Y *= scalar;
	return *this;
}

template<typename T>
ALWAYS_INLINE Vector2T<T> Vector2T<T>::operator/(T scalar) const
{
	return (*this) * (T(1) / scalar);
}

template<typename T>
ALWAYS_INLINE Vector2T<T>& Vector2T<T>::operator/=(T scalar)
{
	return ((*this) *= (T(1) / scalar));
}

template<typename T>
ALWAYS_INLINE T Vector2T<T>::MagnitudeSquared() const
{
	return (X * X) + (Y * Y);
}

template<typename T>
ALWAYS_INLINE T Vector2T<T>::Magnitude() const
{
	return Math::Sqrt(MagnitudeSquared());
}

template<typename T>
ALWAYS_INLINE Vector2T<T> Vector2T<T>::Normalize() const
{
	return (*this) / Magnitude();
}

template<typename T>
ALWAYS_INLINE Vector2T<T> Vector2T<T>::NormalizeIf(T tolerance) const
{
	const T squaredMag = MagnitudeSquared();

	if (Math::AreNearlyEqual<T>(squaredMag, T(1), tolerance))
	{
		return *this;
	}

	return (*this) / Math::Sqrt(squaredMag);
}

template<typename T>
ALWAYS_INLINE Vector2T<T> Vector2T<T>::NormalizeSafe(const Vector2T<T>& errorResult, T threshold) const
{
	const T squaredMag = MagnitudeSquared();

	if (squaredMag < threshold)
	{
		return errorResult;
	}

	return (*this) / Math::Sqrt(squaredMag);
}

template<typename T>
ALWAYS_INLINE Vector2T<T> Vector2T<T>::NormalizeSafeIf(const Vector2T<T>& errorResult, T threshold, T tolerance) const
{
	const T squaredMag = MagnitudeSquared();

	if (squaredMag < threshold)
	{
		return errorResult;
	}

	if (Math::AreNearlyEqual<T>(squaredMag, T(1), tolerance))
	{
		return *this;
	}

	return (*this) / Math::Sqrt(squaredMag);
}

template<typename T>
ALWAYS_INLINE T Vector2T<T>::Dot(const Vector2T<T>& a, const Vector2T<T>& b)
{
	return (a.X * b.X) + (a.Y * b.Y);
}

template<typename T>
ALWAYS_INLINE Vector2T<T> Vector2T<T>::Reflect(const Vector2T<T>& direction, const Vector2T<T>& normal)
{
	return direction - 2 * Dot(direction, normal) * normal;
}

// Vector2 Implementation
#pragma endregion

#pragma region Vector3 Implementation

//////////////// VECTOR3 IMPLEMENTATION ////////////////

template<typename T>
ALWAYS_INLINE Vector3T<T>::Vector3T()
	: X(T(0))
	, Y(T(0))
	, Z(T(0))
{}

template<typename T>
ALWAYS_INLINE Vector3T<T>::Vector3T(const Vector3T<T>& other)
	: X(other.X)
	, Y(other.Y)
	, Z(other.Z)
{}

template<typename T>
ALWAYS_INLINE Vector3T<T>::Vector3T(T x, T y, T z)
	: X(x)
	, Y(y)
	, Z(z)
{}

template<typename T>
ALWAYS_INLINE Vector3T<T>::Vector3T(T scalar)
	: X(scalar)
	, Y(scalar)
	, Z(scalar)
{}

template<typename T>
ALWAYS_INLINE Vector3T<T>::Vector3T(const Vector2T<T>& vector2)
	: X(vector2.X)
	, Y(vector2.Y)
	, Z(T(0))
{}

template<typename T>
ALWAYS_INLINE Vector3T<T>::Vector3T(const Vector2T<T>& vector2, T z)
	: X(vector2.X)
	, Y(vector2.Y)
	, Z(z)
{}

template<typename T>
ALWAYS_INLINE Vector3T<T>::Vector3T(const Vector4T<T>& vector4)
	: X(vector4.X)
	, Y(vector4.Y)
	, Z(vector4.Z)
{}

template<typename T>
Vector3T<T>& Vector3T<T>::operator=(const Vector3T<T>& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
	return *this;
}

template<typename T>
ALWAYS_INLINE Vector3T<T> Vector3T<T>::operator+(const Vector3T<T>& other) const
{
	return Vector3T<T>(X + other.X, Y + other.Y, Z + other.Z);
}

template<typename T>
ALWAYS_INLINE Vector3T<T>& Vector3T<T>::operator+=(const Vector3T<T>& other)
{
	X += other.X;
	Y += other.Y;
	Z += other.Z;
	return *this;
}

template<typename T>
ALWAYS_INLINE Vector3T<T> Vector3T<T>::operator-(const Vector3T<T>& other) const
{
	return Vector3T<T>(X - other.X, Y - other.Y, Z - other.Z);
}

template<typename T>
ALWAYS_INLINE Vector3T<T>& Vector3T<T>::operator-=(const Vector3T<T>& other)
{
	X -= other.X;
	Y -= other.Y;
	Z -= other.Z;
	return *this;
}

template<typename T>
ALWAYS_INLINE Vector3T<T> Vector3T<T>::operator*(T scalar) const
{
	return Vector3T<T>(X * scalar, Y * scalar, Z * scalar);
}

template<typename T>
ALWAYS_INLINE Vector3T<T> operator*(T scalar, const Vector3T<T>& vector)
{
	return vector * scalar;
}

template<typename T>
ALWAYS_INLINE Vector3T<T>& Vector3T<T>::operator*=(T scalar)
{
	X *= scalar;
	Y *= scalar;
	Z *= scalar;
	return *this;
}

template<typename T>
ALWAYS_INLINE Vector3T<T> Vector3T<T>::operator/(T scalar) const
{
	return (*this) * (T(1) / scalar);
}

template<typename T>
ALWAYS_INLINE Vector3T<T>& Vector3T<T>::operator/=(T scalar)
{
	return ((*this) *= (T(1) / scalar));
}

template<typename T>
ALWAYS_INLINE T Vector3T<T>::MagnitudeSquared() const
{
	return (X * X) + (Y * Y) + (Z * Z);
}

template<typename T>
ALWAYS_INLINE T Vector3T<T>::Magnitude() const
{
	return Math::Sqrt(MagnitudeSquared());
}

template<typename T>
ALWAYS_INLINE Vector3T<T> Vector3T<T>::Normalize() const
{
	return (*this) / Magnitude();
}

template<typename T>
ALWAYS_INLINE Vector3T<T> Vector3T<T>::NormalizeIf(T tolerance) const
{
	const T squaredMag = MagnitudeSquared();

	if (Math::AreNearlyEqual<T>(squaredMag, T(1), tolerance))
	{
		return *this;
	}

	return (*this) / Math::Sqrt(squaredMag);
}

template<typename T>
ALWAYS_INLINE Vector3T<T> Vector3T<T>::NormalizeSafe(const Vector3T<T>& errorResult, T threshold) const
{
	const T squaredMag = MagnitudeSquared();

	if (squaredMag < threshold)
	{
		return errorResult;
	}

	return (*this) / Math::Sqrt(squaredMag);
}

template<typename T>
ALWAYS_INLINE Vector3T<T> Vector3T<T>::NormalizeSafeIf(const Vector3T<T>& errorResult, T threshold, T tolerance) const
{
	const T squaredMag = MagnitudeSquared();

	if (squaredMag < threshold)
	{
		return errorResult;
	}

	if (Math::AreNearlyEqual<T>(squaredMag, T(1), tolerance))
	{
		return *this;
	}

	return (*this) / Math::Sqrt(squaredMag);
}

template<typename T>
ALWAYS_INLINE T Vector3T<T>::Dot(const Vector3T<T>& a, const Vector3T<T>& b)
{
	return (a.X * b.X) + (a.Y * b.Y) + (a.Z * b.Z);
}

template<typename T>
ALWAYS_INLINE Vector3T<T> Vector3T<T>::Cross(const Vector3T<T>& a, const Vector3T<T>& b)
{
	return Vector3T<T>
	(
		a.Y * b.Z - a.Z * b.Y,
		a.Z * b.X - a.X * b.Z,
		a.X * b.Y - a.Y * b.X
	);
}

template<typename T>
ALWAYS_INLINE Vector3T<T> Vector3T<T>::Reflect(const Vector3T<T>& direction, const Vector3T<T>& normal)
{
	return direction - 2 * Dot(direction, normal) * normal;
}

// Vector3 Implementation
#pragma endregion

#pragma region Vector4 Implementation

//////////////// VECTOR4 IMPLEMENTATION ////////////////

template<typename T>
ALWAYS_INLINE Vector4T<T>::Vector4T()
	: X(T(0))
	, Y(T(0))
	, Z(T(0))
	, W(T(0))
{}

template<typename T>
ALWAYS_INLINE Vector4T<T>::Vector4T(const Vector4T<T>& other)
	: X(other.X)
	, Y(other.Y)
	, Z(other.Z)
	, W(other.W)
{}

template<typename T>
ALWAYS_INLINE Vector4T<T>::Vector4T(T x, T y, T z, T w)
	: X(x)
	, Y(y)
	, Z(z)
	, W(w)
{}

template<typename T>
ALWAYS_INLINE Vector4T<T>::Vector4T(T scalar)
	: X(scalar)
	, Y(scalar)
	, Z(scalar)
	, W(scalar)
{}

template<typename T>
ALWAYS_INLINE Vector4T<T>::Vector4T(const Vector2T<T>& vector2)
	: X(vector2.X)
	, Y(vector2.Y)
	, Z(T(0))
	, W(T(0))
{}

template<typename T>
ALWAYS_INLINE Vector4T<T>::Vector4T(const Vector2T<T>& vector2, T z, T w)
	: X(vector2.X)
	, Y(vector2.Y)
	, Z(z)
	, W(w)
{}

template<typename T>
ALWAYS_INLINE Vector4T<T>::Vector4T(const Vector3T<T>& vector3)
	: X(vector3.X)
	, Y(vector3.Y)
	, Z(vector3.Z)
	, W(T(0))
{}

template<typename T>
ALWAYS_INLINE Vector4T<T>::Vector4T(const Vector3T<T>& vector3, T w)
	: X(vector3.X)
	, Y(vector3.Y)
	, Z(vector3.Z)
	, W(w)
{}

template<typename T>
ALWAYS_INLINE Vector4T<T>& Vector4T<T>::operator=(const Vector4T<T>& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
	W = other.W;
	return *this;
}

template<typename T>
ALWAYS_INLINE Vector4T<T> Vector4T<T>::operator+(const Vector4T<T>& other) const
{
	return Vector4T<T>(X + other.X, Y + other.Y, Z + other.Z, W + other.W);
}

template<typename T>
ALWAYS_INLINE Vector4T<T>& Vector4T<T>::operator+=(const Vector4T<T>& other)
{
	X += other.X;
	Y += other.Y;
	Z += other.Z;
	W += other.W;
	return *this;
}

template<typename T>
ALWAYS_INLINE Vector4T<T> Vector4T<T>::operator-(const Vector4T<T>& other) const
{
	return Vector4T<T>(X - other.X, Y - other.Y, Z - other.Z, W - other.W);
}

template<typename T>
ALWAYS_INLINE Vector4T<T>& Vector4T<T>::operator-=(const Vector4T<T>& other)
{
	X -= other.X;
	Y -= other.Y;
	Z -= other.Z;
	W -= other.W;
	return *this;
}

template<typename T>
ALWAYS_INLINE Vector4T<T> Vector4T<T>::operator*(T scalar) const
{
	return Vector4T<T>(X * scalar, Y * scalar, Z * scalar, W * scalar);
}

template<typename T>
ALWAYS_INLINE Vector4T<T> operator*(T scalar, const Vector4T<T>& vector)
{
	return vector * scalar;
}

template<typename T>
ALWAYS_INLINE Vector4T<T>& Vector4T<T>::operator*=(T scalar)
{
	X *= scalar;
	Y *= scalar;
	Z *= scalar;
	W *= scalar;
	return *this;
}

template<typename T>
ALWAYS_INLINE Vector4T<T> Vector4T<T>::operator/(T scalar) const
{
	return (*this) * (T(1) / scalar);
}

template<typename T>
ALWAYS_INLINE Vector4T<T>& Vector4T<T>::operator/=(T scalar)
{
	return ((*this) *= (T(1) / scalar));
}

// Vector4 Implementation
#pragma endregion

#pragma region Ray

/**
 *----------------------------------------------------------------
 * Hiccup Ray.
 *----------------------------------------------------------------
 */
template<typename T>
struct RayT
{
public:
	// The ray's origin.
	Vector3T<T> Origin;

	// The ray's direction.
	Vector3T<T> Direction;

public:
	/**
	 * Default constructor.
	 */
	ALWAYS_INLINE RayT();
};

// Ray, with 32-bit floating point precision.
using Rayf = RayT<float32>;

// Ray, with 64-bit floating point precision.
using Rayd = RayT<float64>;

// Ray, with 32-bit unsigned integer precision.
using Rayu = RayT<uint32>;

// Ray, with 32-bit signed integer precision.
using Rayi = RayT<int32>;

// Ray, with floating point precision.
using Ray = Rayf;

// Ray
#pragma endregion

#pragma region Ray Implementation

template<typename T>
ALWAYS_INLINE RayT<T>::RayT()
	: Origin()
	, Direction()
{}

// Ray Implementation
#pragma endregion

#pragma region AABB2

/**
 *----------------------------------------------------------------
 * Hiccup Axis-Aligned Bounding Box (AABB).
 *----------------------------------------------------------------
 */
template<typename T>
struct AABB2T
{
public:
	// The upper bound point.
	Vector2T<T> MinBound;

	// The lower bound point.
	Vector2T<T> MaxBound;

public:
	/**
	 * Default constructor.
	 */
	ALWAYS_INLINE AABB2T();

public:
	ALWAYS_INLINE bool ContainsPoint(const Vector2T<T>& point) const;
};

using AABB2f = AABB2T<float32>;
using AABB2d = AABB2T<float64>;
using AABB2u = AABB2T<uint32>;
using AABB2i = AABB2T<int32>;

using AABB2 = AABB2f;

// AABB2
#pragma endregion

#pragma region AABB3

/**
 *----------------------------------------------------------------
 * Hiccup Axis-Aligned Bounding Box (AABB).
 *----------------------------------------------------------------
 */
template<typename T>
struct AABB3T
{
public:
	// The upper bound point.
	Vector3T<T> MinBound;

	// The lower bound point.
	Vector3T<T> MaxBound;

public:
	/**
	 * Default constructor.
	 */
	ALWAYS_INLINE AABB3T();

public:
	ALWAYS_INLINE bool ContainsPoint(const Vector3T<T>& point) const;
};

using AABB3f    = AABB3T<float32>;
using AABB3d    = AABB3T<float64>;
using AABB3u    = AABB3T<uint32>;
using AABB3i    = AABB3T<int32>;

using AABB3     = AABB3f;

// AABB3
#pragma endregion

#pragma region AABB2 Implementation

template<typename T>
ALWAYS_INLINE AABB2T<T>::AABB2T()
	: MinBound(T(+1))
	, MaxBound(T(-1))
{}

template<typename T>
ALWAYS_INLINE bool AABB2T<T>::ContainsPoint(const Vector2T<T>& point) const
{
	return
		MinBound.X <= point.X && point.X <= MaxBound.X &&
		MinBound.Y <= point.Y && point.Y <= MaxBound.Y;
}

// AABB2 Implementation
#pragma endregion

#pragma region AABB3 Implementation

template<typename T>
ALWAYS_INLINE AABB3T<T>::AABB3T()
	: MinBound(T(+1))
	, MaxBound(T(-1))
{}

template<typename T>
ALWAYS_INLINE bool AABB3T<T>::ContainsPoint(const Vector3T<T>& point) const
{
	return
		MinBound.X <= point.X && point.X <= MaxBound.X &&
		MinBound.Y <= point.Y && point.Y <= MaxBound.Y &&
		MinBound.Z <= point.Z && point.Z <= MaxBound.Z;
}

// AABB3 Implementation
#pragma endregion

} // namespace HC