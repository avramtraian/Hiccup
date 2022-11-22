// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"

namespace HC
{

/**
 *----------------------------------------------------------------
 * Hiccup Math.
 *----------------------------------------------------------------
*/
struct Math
{
public:
	template<typename T>
	FORCEINLINE static T Max(T a, T b)
	{
		return (a > b) ? a : b;
	}
};

} // namespace HC