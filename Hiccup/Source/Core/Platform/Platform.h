// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"

namespace HC
{

struct PlatformSpecification
{
	bool IsConsoleAttached;
};

class Platform
{
public:
	static bool Initialize(const PlatformSpecification& specification);
	static void Shutdown();

public:
	static void* Allocate(usize bytesCount);

	static void Free(void* memoryBlock);
};

} // namespace HC