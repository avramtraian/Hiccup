// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "MathUtilities.h"

#include <cmath>

namespace HC
{

float32 Math::sqrt(float32 x)
{
	return std::sqrtf(x);
}

float64 Math::sqrt(float64 x)
{
	return std::sqrt(x);
}

float32 Math::sin(float32 x)
{
	return std::sinf(x);
}

float64 Math::sin(float64 x)
{
	return std::sin(x);
}

float32 Math::cos(float32 x)
{
	return std::cosf(x);
}

float64 Math::cos(float64 x)
{
	return std::cos(x);
}

float32 Math::tan(float32 x)
{
	return std::tanf(x);
}

float64 Math::tan(float64 x)
{
	return std::tan(x);
}

float32 Math::asin(float32 x)
{
	return std::asinf(x);
}

float64 Math::asin(float64 x)
{
	return std::asin(x);
}

float32 Math::acos(float32 x)
{
	return std::acosf(x);
}

float64 Math::acos(float64 x)
{
	return std::acos(x);
}

float32 Math::atan(float32 x)
{
	return std::atanf(x);
}

float64 Math::atan(float64 x)
{
	return std::atan(x);
}

} // namespace HC