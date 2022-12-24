// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "MathUtilities.h"

#include <cmath>

namespace HC
{

float32_t Math::sqrt(float32_t x)
{
	return std::sqrtf(x);
}

float64_t Math::sqrt(float64_t x)
{
	return std::sqrt(x);
}

float32_t Math::sin(float32_t x)
{
	return std::sinf(x);
}

float64_t Math::sin(float64_t x)
{
	return std::sin(x);
}

float32_t Math::cos(float32_t x)
{
	return std::cosf(x);
}

float64_t Math::cos(float64_t x)
{
	return std::cos(x);
}

float32_t Math::tan(float32_t x)
{
	return std::tanf(x);
}

float64_t Math::tan(float64_t x)
{
	return std::tan(x);
}

float32_t Math::asin(float32_t x)
{
	return std::asinf(x);
}

float64_t Math::asin(float64_t x)
{
	return std::asin(x);
}

float32_t Math::acos(float32_t x)
{
	return std::acosf(x);
}

float64_t Math::acos(float64_t x)
{
	return std::acos(x);
}

float32_t Math::atan(float32_t x)
{
	return std::atanf(x);
}

float64_t Math::atan(float64_t x)
{
	return std::atan(x);
}

} // namespace HC