// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "MathUtilities.h"

#include <cmath>
#include <cstdlib>

namespace HC
{

float32 Math::Sqrt(float32 x)
{
	return std::sqrtf(x);
}

float64 Math::Sqrt(float64 x)
{
	return std::sqrt(x);
}

float32 Math::Sin(float32 x)
{
	return std::sinf(x);
}

float64 Math::Sin(float64 x)
{
	return std::sin(x);
}

float32 Math::Cos(float32 x)
{
	return std::cosf(x);
}

float64 Math::Cos(float64 x)
{
	return std::cos(x);
}

float32 Math::Tan(float32 x)
{
	return std::tanf(x);
}

float64 Math::Tan(float64 x)
{
	return std::tan(x);
}

float32 Math::Asin(float32 x)
{
	return std::asinf(x);
}

float64 Math::Asin(float64 x)
{
	return std::asin(x);
}

float32 Math::Acos(float32 x)
{
	return std::acosf(x);
}

float64 Math::Acos(float64 x)
{
	return std::acos(x);
}

float32 Math::Atan(float32 x)
{
	return std::atanf(x);
}

float64 Math::Atan(float64 x)
{
	return std::atan(x);
}

} // namespace HC