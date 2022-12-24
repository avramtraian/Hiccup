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
	T x;

	// The vector's component on the Y-axis.
	T y;

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
	 * @param in_x The component on the X-axis.
	 * @param in_y The component on the Y-axis.
	 */
	ALWAYS_INLINE Vector2T(T in_x, T in_y);

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
	ALWAYS_INLINE T magnitude_squared() const;

	/** @return The magnitude of the vector. */
	ALWAYS_INLINE T magnitude() const;

	/**
	 * Normalizes this vector.
	 *
	 * @return The vector's normal.
	 */
	ALWAYS_INLINE Vector2T<T> normalize() const;

	/**
	 * Normalizes this vector, the calculation being made only if the vector is not already normalized.
	 *
	 * @param tolerance The maximum allowed absolute difference between the vector's squared magnitude and 1
	 *   for which the vector is considered already normalized.
	 *
	 * @return The vector's normal.
	 */
	ALWAYS_INLINE Vector2T<T> normalize_if(T tolerance = T(KINDA_SMALL_NUMBER)) const;

	/**
	 * Normalizes this vector, only if it is safe to do it (its magnitude is not 0).
	 *
	 * @param error_result The value to return when the normal doesn't exist.
	 * @param threshold The minimum allowed value for the vector's squared magnitude for which the vector's
	 *   normal exists.
	 *
	 * @return The vector's normal (if possible) or the specified result for error.
	 */
	ALWAYS_INLINE Vector2T<T> normalize_safe(const Vector2T<T>& error_result, T threshold = T(KINDA_SMALL_NUMBER)) const;

	/**
	 * Normalizes this vector, only if it is safe to do it (its magnitude is not 0) and the vector isn't already
	 *   normalized.
	 *
	 * @param error_result The value to return when the normal doesn't exist.
	 * @param threshold The minimum allowed value for the vector's squared magnitude for which the vector's
	 *   normal exists.
	 * @param tolerance The maximum allowed absolute difference between the vector's squared magnitude and 1
	 *   for which the vector is considered already normalized.
	 *
	 * @return The vector's normal (if possible) or the specified result for error.
	 */
	ALWAYS_INLINE Vector2T<T> normalize_safe_if(const Vector2T<T>& error_result, T threshold = T(KINDA_SMALL_NUMBER), T tolerance = T(KINDA_SMALL_NUMBER)) const;

	/**
	 * Calculates the dot product between two vectors.
	 *
	 * @param a The first vector.
	 * @param b The second vector.
	 *
	 * @return The dot product.
	 */
	ALWAYS_INLINE static T dot(const Vector2T<T>& a, const Vector2T<T>& b);

public:
	/**
	 * Reflects a vector based on a normal.
	 *
	 * @param direction The direction to reflect.
	 * @param normal The normal to reflect against.
	 *
	 * @return The reflected direction.
	 */
	ALWAYS_INLINE static Vector2T<T> reflect(const Vector2T<T>& direction, const Vector2T<T>& normal);
};

// 2-Component vector, with 32-bit floating point precision.
using Vector2f  = Vector2T<float32_t>;

// 2-Component vector, with 64-bit floating point precision.
using Vector2d  = Vector2T<float64_t>;

// 2-Component vector, with 32-bit unsigned integer precision.
using Vector2u  = Vector2T<uint32_t>;

// 2-Component vector, with 32-bit signed integer precision.
using Vector2i  = Vector2T<int32_t>;

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
	T x;

	// The vector's component on the Y-axis.
	T y;

	// The vector's component on the Z-axis.
	T z;

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
	 * @param in_x The component on the X-axis.
	 * @param in_y The component on the Y-axis.
	 * @param in_z The component on the Z-axis.
	 */
	ALWAYS_INLINE Vector3T(T in_x, T in_y, T in_z);

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
	 * @param in_z The value to initialize the vector's Z component with.
	 */
	ALWAYS_INLINE Vector3T(const Vector2T<T>& vector2, T in_z);
	
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
	ALWAYS_INLINE T magnitude_squared() const;

	/** @return The magnitude of the vector. */
	ALWAYS_INLINE T magnitude() const;

	/**
	 * Normalizes this vector.
	 * 
	 * @return The vector's normal.
	 */
	ALWAYS_INLINE Vector3T<T> normalize() const;

	/**
	 * Normalizes this vector, the calculation being made only if the vector is not already normalized.
	 * 
	 * @param tolerance The maximum allowed absolute difference between the vector's squared magnitude and 1
	 *   for which the vector is considered already normalized.
	 * 
	 * @return The vector's normal.
	 */
	ALWAYS_INLINE Vector3T<T> normalize_if(T tolerance = T(KINDA_SMALL_NUMBER)) const;

	/**
	 * Normalizes this vector, only if it is safe to do it (its magnitude is not 0).
	 * 
	 * @param error_result The value to return when the normal doesn't exist.
	 * @param threshold The minimum allowed value for the vector's squared magnitude for which the vector's
	 *   normal exists.
	 * 
	 * @return The vector's normal (if possible) or the specified result for error.
	 */
	ALWAYS_INLINE Vector3T<T> normalize_safe(const Vector3T<T>& error_result, T threshold = T(KINDA_SMALL_NUMBER)) const;

	/**
	 * Normalizes this vector, only if it is safe to do it (its magnitude is not 0) and the vector isn't already
	 *   normalized.
	 * 
	 * @param error_result The value to return when the normal doesn't exist.
	 * @param threshold The minimum allowed value for the vector's squared magnitude for which the vector's
	 *   normal exists.
	 * @param tolerance The maximum allowed absolute difference between the vector's squared magnitude and 1
	 *   for which the vector is considered already normalized.
	 * 
	 * @return The vector's normal (if possible) or the specified result for error.
	 */
	ALWAYS_INLINE Vector3T<T> normalize_safe_if(const Vector3T<T>& error_result, T threshold = T(KINDA_SMALL_NUMBER), T tolerance = T(KINDA_SMALL_NUMBER)) const;

	/**
	 * Calculates the dot product between two vectors.
	 * 
	 * @param a The first vector.
	 * @param b The second vector.
	 * 
	 * @return The dot product.
	 */
	ALWAYS_INLINE static T dot(const Vector3T<T>& a, const Vector3T<T>& b);

	/**
	 * Calculates the cross product between two vectors.
	 * 
	 * @param a The first vector.
	 * @param b The second vector.
	 * 
	 * @return The cross product.
	 */
	ALWAYS_INLINE static Vector3T<T> cross(const Vector3T<T>& a, const Vector3T<T>& b);

public:
	/**
	 * Reflects a vector based on a normal.
	 * 
	 * @param direction The direction to reflect.
	 * @param normal The normal to reflect against.
	 * 
	 * @return The reflected direction.
	 */
	ALWAYS_INLINE static Vector3T<T> reflect(const Vector3T<T>& direction, const Vector3T<T>& normal);
};

// 3-Component vector, with 32-bit floating point precision.
using Vector3f  = Vector3T<float32_t>;

// 3-Component vector, with 64-bit floating point precision.
using Vector3d  = Vector3T<float64_t>;

// 3-Component vector, with 32-bit unsigned integer precision.
using Vector3u  = Vector3T<uint32_t>;

// 3-Component vector, with 32-bit signed integer precision.
using Vector3i  = Vector3T<int32_t>;

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
	T x;

	// The vector's component on the Y-axis.
	T y;

	// The vector's component on the Z-axis.
	T z;

	// The vector's component on the W-axis.
	T w;

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
	 * @param in_x The component on the X-axis.
	 * @param in_y The component on the Y-axis.
	 * @param in_z The component on the Z-axis.
	 * @param in_w The component on the W-axis.
	 */
	ALWAYS_INLINE Vector4T(T in_x, T in_y, T in_z, T in_w);

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
	 * @param in_z The value to initialize the vector's Z component with.
	 * @param in_w The value to initialize the vector's W component with.
	 */
	ALWAYS_INLINE Vector4T(const Vector2T<T>& vector2, T in_z, T in_w);

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
	 * @param in_w The value to initialize the vector's W component with.
	 */
	ALWAYS_INLINE Vector4T(const Vector3T<T>& vector3, T in_w);

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
using Vector4f  = Vector4T<float32_t>;

// 4-Component vector, with 64-bit floating point precision.
using Vector4d  = Vector4T<float64_t>;

// 4-Component vector, with 32-bit unsigned integer precision.
using Vector4u  = Vector4T<uint32_t>;

// 4-Component vector, with 32-bit signed integer precision.
using Vector4i  = Vector4T<int32_t>;

// 4-Component vector, with floating point precision.
using Vector4   = Vector4f;

// Vector4
#pragma endregion

#pragma region Vector2 Implementation

//////////////// VECTOR2 IMPLEMENTATION ////////////////

template<typename T>
ALWAYS_INLINE Vector2T<T>::Vector2T()
	: x(T(0))
	, y(T(0))
{}

template<typename T>
ALWAYS_INLINE Vector2T<T>::Vector2T(const Vector2T<T>& other)
	: x(other.x)
	, y(other.y)
{}

template<typename T>
ALWAYS_INLINE Vector2T<T>::Vector2T(T in_x, T in_y)
	: x(in_x)
	, y(in_y)
{}

template<typename T>
ALWAYS_INLINE Vector2T<T>::Vector2T(T scalar)
	: x(scalar)
	, y(scalar)
{}

template<typename T>
ALWAYS_INLINE Vector2T<T>::Vector2T(const Vector3T<T>& vector3)
	: x(vector3.x)
	, y(vector3.y)
{}

template<typename T>
ALWAYS_INLINE Vector2T<T>::Vector2T(const Vector4T<T>& vector4)
	: x(vector4.x)
	, y(vector4.y)
{}

template<typename T>
ALWAYS_INLINE Vector2T<T>& Vector2T<T>::operator=(const Vector2T<T>& other)
{
	x = other.x;
	y = other.y;
	return *this;
}

template<typename T>
ALWAYS_INLINE Vector2T<T> Vector2T<T>::operator+(const Vector2T<T>& other) const
{
	return Vector2T<T>(x + other.x, y + other.y);
}

template<typename T>
ALWAYS_INLINE Vector2T<T>& Vector2T<T>::operator+=(const Vector2T<T>& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

template<typename T>
ALWAYS_INLINE Vector2T<T> Vector2T<T>::operator-(const Vector2T<T>& other) const
{
	return Vector2T<T>(x - other.x, y - other.y);
}

template<typename T>
ALWAYS_INLINE Vector2T<T>& Vector2T<T>::operator-=(const Vector2T<T>& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

template<typename T>
ALWAYS_INLINE Vector2T<T> Vector2T<T>::operator*(T scalar) const
{
	return Vector2T<T>(x * scalar, y * scalar);
}

template<typename T>
ALWAYS_INLINE Vector2T<T> operator*(T scalar, const Vector2T<T>& vector)
{
	return vector * scalar;
}

template<typename T>
ALWAYS_INLINE Vector2T<T>& Vector2T<T>::operator*=(T scalar)
{
	x *= scalar;
	y *= scalar;
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
ALWAYS_INLINE T Vector2T<T>::magnitude_squared() const
{
	return (x * x) + (y * y);
}

template<typename T>
ALWAYS_INLINE T Vector2T<T>::magnitude() const
{
	return Math::sqrt(magnitude_squared());
}

template<typename T>
ALWAYS_INLINE Vector2T<T> Vector2T<T>::normalize() const
{
	return (*this) / magnitude();
}

template<typename T>
ALWAYS_INLINE Vector2T<T> Vector2T<T>::normalize_if(T tolerance) const
{
	const T squared_mag = magnitude_squared();

	if (Math::are_nearly_equal<T>(squared_mag, T(1), tolerance))
	{
		return *this;
	}

	return (*this) / Math::sqrt(squared_mag);
}

template<typename T>
ALWAYS_INLINE Vector2T<T> Vector2T<T>::normalize_safe(const Vector2T<T>& error_result, T threshold) const
{
	const T squared_mag = magnitude_squared();

	if (squared_mag < threshold)
	{
		return error_result;
	}

	return (*this) / Math::sqrt(squared_mag);
}

template<typename T>
ALWAYS_INLINE Vector2T<T> Vector2T<T>::normalize_safe_if(const Vector2T<T>& error_result, T threshold, T tolerance) const
{
	const T squared_mag = magnitude_squared();

	if (squared_mag < threshold)
	{
		return error_result;
	}

	if (Math::are_nearly_equal<T>(squared_mag, T(1), tolerance))
	{
		return *this;
	}

	return (*this) / Math::sqrt(squared_mag);
}

template<typename T>
ALWAYS_INLINE T Vector2T<T>::dot(const Vector2T<T>& a, const Vector2T<T>& b)
{
	return (a.x * b.x) + (a.y * b.y);
}

template<typename T>
ALWAYS_INLINE Vector2T<T> Vector2T<T>::reflect(const Vector2T<T>& direction, const Vector2T<T>& normal)
{
	return direction - 2 * dot(direction, normal) * normal;
}

// Vector2 Implementation
#pragma endregion

#pragma region Vector3 Implementation

//////////////// VECTOR3 IMPLEMENTATION ////////////////

template<typename T>
ALWAYS_INLINE Vector3T<T>::Vector3T()
	: x(T(0))
	, y(T(0))
	, z(T(0))
{}

template<typename T>
ALWAYS_INLINE Vector3T<T>::Vector3T(const Vector3T<T>& other)
	: x(other.x)
	, y(other.y)
	, z(other.z)
{}

template<typename T>
ALWAYS_INLINE Vector3T<T>::Vector3T(T in_x, T in_y, T in_z)
	: x(in_x)
	, y(in_y)
	, z(in_z)
{}

template<typename T>
ALWAYS_INLINE Vector3T<T>::Vector3T(T scalar)
	: x(scalar)
	, y(scalar)
	, z(scalar)
{}

template<typename T>
ALWAYS_INLINE Vector3T<T>::Vector3T(const Vector2T<T>& vector2)
	: x(vector2.x)
	, y(vector2.y)
	, z(T(0))
{}

template<typename T>
ALWAYS_INLINE Vector3T<T>::Vector3T(const Vector2T<T>& vector2, T in_z)
	: x(vector2.x)
	, y(vector2.y)
	, z(in_z)
{}

template<typename T>
ALWAYS_INLINE Vector3T<T>::Vector3T(const Vector4T<T>& vector4)
	: x(vector4.x)
	, y(vector4.y)
	, z(vector4.z)
{}

template<typename T>
Vector3T<T>& Vector3T<T>::operator=(const Vector3T<T>& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

template<typename T>
ALWAYS_INLINE Vector3T<T> Vector3T<T>::operator+(const Vector3T<T>& other) const
{
	return Vector3T<T>(x + other.x, y + other.y, z + other.z);
}

template<typename T>
ALWAYS_INLINE Vector3T<T>& Vector3T<T>::operator+=(const Vector3T<T>& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

template<typename T>
ALWAYS_INLINE Vector3T<T> Vector3T<T>::operator-(const Vector3T<T>& other) const
{
	return Vector3T<T>(x - other.x, y - other.y, z - other.z);
}

template<typename T>
ALWAYS_INLINE Vector3T<T>& Vector3T<T>::operator-=(const Vector3T<T>& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

template<typename T>
ALWAYS_INLINE Vector3T<T> Vector3T<T>::operator*(T scalar) const
{
	return Vector3T<T>(x * scalar, y * scalar, z * scalar);
}

template<typename T>
ALWAYS_INLINE Vector3T<T> operator*(T scalar, const Vector3T<T>& vector)
{
	return vector * scalar;
}

template<typename T>
ALWAYS_INLINE Vector3T<T>& Vector3T<T>::operator*=(T scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
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
ALWAYS_INLINE T Vector3T<T>::magnitude_squared() const
{
	return (x * x) + (y * y) + (z * z);
}

template<typename T>
ALWAYS_INLINE T Vector3T<T>::magnitude() const
{
	return Math::sqrt(magnitude_squared());
}

template<typename T>
ALWAYS_INLINE Vector3T<T> Vector3T<T>::normalize() const
{
	return (*this) / magnitude();
}

template<typename T>
ALWAYS_INLINE Vector3T<T> Vector3T<T>::normalize_if(T tolerance) const
{
	const T squared_mag = magnitude_squared();

	if (Math::are_nearly_equal<T>(squared_mag, T(1), tolerance))
	{
		return *this;
	}

	return (*this) / Math::sqrt(squared_mag);
}

template<typename T>
ALWAYS_INLINE Vector3T<T> Vector3T<T>::normalize_safe(const Vector3T<T>& error_result, T threshold) const
{
	const T squared_mag = magnitude_squared();

	if (squared_mag < threshold)
	{
		return error_result;
	}

	return (*this) / Math::sqrt(squared_mag);
}

template<typename T>
ALWAYS_INLINE Vector3T<T> Vector3T<T>::normalize_safe_if(const Vector3T<T>& error_result, T threshold, T tolerance) const
{
	const T squared_mag = magnitude_squared();

	if (squared_mag < threshold)
	{
		return error_result;
	}

	if (Math::are_nearly_equal<T>(squared_mag, T(1), tolerance))
	{
		return *this;
	}

	return (*this) / Math::sqrt(squared_mag);
}

template<typename T>
ALWAYS_INLINE T Vector3T<T>::dot(const Vector3T<T>& a, const Vector3T<T>& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

template<typename T>
ALWAYS_INLINE Vector3T<T> Vector3T<T>::cross(const Vector3T<T>& a, const Vector3T<T>& b)
{
	return Vector3T<T>
	(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}

template<typename T>
ALWAYS_INLINE Vector3T<T> Vector3T<T>::reflect(const Vector3T<T>& direction, const Vector3T<T>& normal)
{
	return direction - 2 * dot(direction, normal) * normal;
}

// Vector3 Implementation
#pragma endregion

#pragma region Vector4 Implementation

//////////////// VECTOR4 IMPLEMENTATION ////////////////

template<typename T>
ALWAYS_INLINE Vector4T<T>::Vector4T()
	: x(T(0))
	, y(T(0))
	, z(T(0))
	, w(T(0))
{}

template<typename T>
ALWAYS_INLINE Vector4T<T>::Vector4T(const Vector4T<T>& other)
	: x(other.x)
	, y(other.y)
	, z(other.z)
	, w(other.w)
{}

template<typename T>
ALWAYS_INLINE Vector4T<T>::Vector4T(T in_x, T in_y, T in_z, T in_w)
	: x(in_x)
	, y(in_y)
	, z(in_z)
	, w(in_w)
{}

template<typename T>
ALWAYS_INLINE Vector4T<T>::Vector4T(T scalar)
	: x(scalar)
	, y(scalar)
	, z(scalar)
	, w(scalar)
{}

template<typename T>
ALWAYS_INLINE Vector4T<T>::Vector4T(const Vector2T<T>& vector2)
	: x(vector2.x)
	, y(vector2.y)
	, z(T(0))
	, w(T(0))
{}

template<typename T>
ALWAYS_INLINE Vector4T<T>::Vector4T(const Vector2T<T>& vector2, T in_z, T in_w)
	: x(vector2.x)
	, y(vector2.y)
	, z(in_z)
	, w(in_w)
{}

template<typename T>
ALWAYS_INLINE Vector4T<T>::Vector4T(const Vector3T<T>& vector3)
	: x(vector3.x)
	, y(vector3.y)
	, z(vector3.z)
	, w(T(0))
{}

template<typename T>
ALWAYS_INLINE Vector4T<T>::Vector4T(const Vector3T<T>& vector3, T in_w)
	: x(vector3.x)
	, y(vector3.y)
	, z(vector3.z)
	, w(in_w)
{}

template<typename T>
ALWAYS_INLINE Vector4T<T>& Vector4T<T>::operator=(const Vector4T<T>& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
	return *this;
}

template<typename T>
ALWAYS_INLINE Vector4T<T> Vector4T<T>::operator+(const Vector4T<T>& other) const
{
	return Vector4T<T>(x + other.x, y + other.y, z + other.z, w + other.w);
}

template<typename T>
ALWAYS_INLINE Vector4T<T>& Vector4T<T>::operator+=(const Vector4T<T>& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

template<typename T>
ALWAYS_INLINE Vector4T<T> Vector4T<T>::operator-(const Vector4T<T>& other) const
{
	return Vector4T<T>(x - other.x, y - other.y, z - other.z, w - other.w);
}

template<typename T>
ALWAYS_INLINE Vector4T<T>& Vector4T<T>::operator-=(const Vector4T<T>& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}

template<typename T>
ALWAYS_INLINE Vector4T<T> Vector4T<T>::operator*(T scalar) const
{
	return Vector4T<T>(x * scalar, y * scalar, z * scalar, w * scalar);
}

template<typename T>
ALWAYS_INLINE Vector4T<T> operator*(T scalar, const Vector4T<T>& vector)
{
	return vector * scalar;
}

template<typename T>
ALWAYS_INLINE Vector4T<T>& Vector4T<T>::operator*=(T scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
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
	Vector3T<T> origin;

	// The ray's direction.
	Vector3T<T> direction;

public:
	/**
	 * Default constructor.
	 */
	ALWAYS_INLINE RayT();
};

// Ray, with 32-bit floating point precision.
using Rayf = RayT<float32_t>;

// Ray, with 64-bit floating point precision.
using Rayd = RayT<float64_t>;

// Ray, with 32-bit unsigned integer precision.
using Rayu = RayT<uint32_t>;

// Ray, with 32-bit signed integer precision.
using Rayi = RayT<int32_t>;

// Ray, with floating point precision.
using Ray = Rayf;

// Ray
#pragma endregion

#pragma region Ray Implementation

template<typename T>
ALWAYS_INLINE RayT<T>::RayT()
	: origin()
	, direction()
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
	Vector2T<T> min_bound;

	// The lower bound point.
	Vector2T<T> max_bound;

public:
	/**
	 * Default constructor.
	 */
	ALWAYS_INLINE AABB2T();

public:
	ALWAYS_INLINE bool contains_point(const Vector2T<T>& point) const;
};

using AABB2f = AABB2T<float32_t>;
using AABB2d = AABB2T<float64_t>;
using AABB2u = AABB2T<uint32_t>;
using AABB2i = AABB2T<int32_t>;

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
	Vector3T<T> min_bound;

	// The lower bound point.
	Vector3T<T> max_bound;

public:
	/**
	 * Default constructor.
	 */
	ALWAYS_INLINE AABB3T();

public:
	ALWAYS_INLINE bool contains_point(const Vector3T<T>& point) const;
};

using AABB3f    = AABB3T<float32_t>;
using AABB3d    = AABB3T<float64_t>;
using AABB3u    = AABB3T<uint32_t>;
using AABB3i    = AABB3T<int32_t>;

using AABB3     = AABB3f;

// AABB3
#pragma endregion

#pragma region AABB2 Implementation

template<typename T>
ALWAYS_INLINE AABB2T<T>::AABB2T()
	: min_bound(T(+1))
	, max_bound(T(-1))
{}

template<typename T>
ALWAYS_INLINE bool AABB2T<T>::contains_point(const Vector2T<T>& point) const
{
	return
		min_bound.x <= point.x && point.x <= max_bound.x &&
		min_bound.y <= point.y && point.y <= max_bound.y;
}

// AABB2 Implementation
#pragma endregion

#pragma region AABB3 Implementation

template<typename T>
ALWAYS_INLINE AABB3T<T>::AABB3T()
	: min_bound(T(+1))
	, max_bound(T(-1))
{}

template<typename T>
ALWAYS_INLINE bool AABB3T<T>::contains_point(const Vector3T<T>& point) const
{
	return
		min_bound.x <= point.x && point.x <= max_bound.x &&
		min_bound.y <= point.y && point.y <= max_bound.y &&
		min_bound.z <= point.z && point.z <= max_bound.z;
}

// AABB3 Implementation
#pragma endregion

} // namespace HC