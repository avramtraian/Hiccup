// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"

namespace HC
{

/**
 *----------------------------------------------------------------
 * Hiccup Default Comparator.
 *----------------------------------------------------------------
 * Used, by default, in most containers where element comparasion is required.
 */
class DefaultComparator
{
public:
	template<typename T>
	ALWAYS_INLINE static uint64 Compare(const T& a, const T& b)
	{
		return (a == b);
	}
};

} // namespace HC