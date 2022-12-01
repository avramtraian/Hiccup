// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"

#include "MathUtilities.h"
#include "Geometry.h"

namespace HC
{

#pragma region Sphere Intersection

namespace Utils 
{

// Computes the coefficients and discriminant for solving the ray-sphere intersection equation.
template<typename T>
ALWAYS_INLINE void InterectSphereCoefficients(const RayT<T>& ray, const Vector3T<T>& sphereOrigin, T sphereRadius, T& b, T& discriminant)
{
	// a = Dot(ray.Direction, ray.Direction) = 1
	b = Vector3T<T>::Dot(ray.Origin, ray.Direction) - Vector3T<T>::Dot(ray.Direction, sphereOrigin);
	const T c =
		Vector3T<T>::Dot(ray.Origin, ray.Origin) +
		Vector3T<T>::Dot(sphereOrigin, sphereOrigin) -
		T(2) * Vector3T<T>::Dot(sphereOrigin, ray.Origin) -
		(sphereRadius * sphereRadius);

	discriminant = b * b - c;
}

} // namespace HC::Utils

/**
 * Calculates the intersection points with a sphere, if any.
 * 
 * @param ray The ray on which the intersection point should be.
 * @param sphere sphereOrigin The world position of the sphere origin.
 * @param sphereRadius The radius of the sphere.
 * @param outT1 The distance along the ray where the closest intersection point is.
 *   This will contain a valid value only if the ray hits the sphere.
 * @param outT2 The distance along the ray where the furthest intersection point is.
 *   This will contain a valid value only if the ray hits the sphere twice.
 * 
 * @return The number of intersection points. This can be 0 - no hits, 1 - single hit, 2 - double hit.
 */
template<typename T>
ALWAYS_INLINE uint8 InterectSphere(const RayT<T>& ray, const Vector3T<T>& sphereOrigin, T sphereRadius, T& outT1, T& outT2)
{
	T b = {};
	T discriminant = {};
	Utils::InterectSphereCoefficients(ray, sphereOrigin, sphereRadius, b, discriminant);

	if (discriminant < T(0))
	{
		return 0;
	}

	if (Math::AreNearlyEqual(discriminant, 0))
	{
		outT1 = -b;
		return 1;
	}
	
	const T discriminantRoot = Math::Sqrt(discriminant);
	outT1 = (-b - discriminantRoot);
	outT2 = (-b + discriminantRoot);
	return 2;
}

/**
 * Calculates the closest intersection point with a sphere, if any.
 * 
 * @param ray The ray on which the intersection point should be.
 * @param sphere sphereOrigin The world position of the sphere origin.
 * @param sphereRadius The radius of the sphere.
 * @param outT The distance along the ray where the closest intersection point is.
 *   This will contain a valid value only if the ray hits the sphere.
 * 
 * @return True if the ray hits the sphere (and thus there is an intersection point); False otherwise.
 */
template<typename T>
ALWAYS_INLINE bool InterectSphere(const RayT<T>& ray, const Vector3T<T>& sphereOrigin, T sphereRadius, T& outT)
{
	T b = {};
	T discriminant = {};
	Utils::InterectSphereCoefficients(ray, sphereOrigin, sphereRadius, b, discriminant);

	if (discriminant < T(0))
	{
		return false;
	}

	outT = (-b - Math::Sqrt(discriminant));
	return true;
}

/**
 * Checks to see if a ray intersects a sphere, without computing the intersection points.
 * 
 * @param ray The ray to check intersection against.
 * @param sphereOrigin The world position of the sphere origin.
 * @param sphereRadius The radius of the sphere.
 * 
 * @return True if the ray hits the sphere; False otherwise.
 */
template<typename T>
ALWAYS_INLINE bool InterectSphere(const RayT<T>& ray, const Vector3T<T>& sphereOrigin, T sphereRadius)
{
	T b = {};
	T discriminant = {};
	Utils::InterectSphereCoefficients(ray, sphereOrigin, sphereRadius, b, discriminant);

	return (discriminant >= T(0));
}

// Sphere Intersection
#pragma endregion

} // namespace HC