// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreDefines.h"

#define HC_CORE_ASSERT(EXPRESSION)          \
	if (!(EXPRESSION))                      \
	{                                       \
		HC_DEBUGBREAK;                      \
	}

#define HC_CORE_ASSERTF(EXPRESSION, ...)    \
	if (!(EXPRESSION))                      \
	{                                       \
		                                    \
		HC_DEBUGBREAK;                      \
	}