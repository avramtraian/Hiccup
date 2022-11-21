// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Memory.h"

namespace HC
{

struct HC_API Buffer
{
public:
	uint8* Data;
	usize  Size;

public:
	FORCEINLINE Buffer()
		: Data(nullptr)
		, Size(0)
	{}

	FORCEINLINE Buffer(usize size)
		: Data(nullptr)
		, Size(0)
	{
		Allocate(size);
	}

	FORCEINLINE Buffer(const Buffer& other) = default;

public:
	template<typename T>
	FORCEINLINE T* As() { return (T*)Data; }

	template<typename T>
	FORCEINLINE const T* As() const { return (const T*)Data; }

	FORCEINLINE static Buffer Copy(Buffer source)
	{
		Buffer destination = Buffer(source.Size);
		Memory::Copy(destination.Data, source.Data, source.Size);
		return destination;
	}

public:
	FORCEINLINE void Allocate(usize size)
	{
		if (Data)
		{
			Release();
		}

		Data = HcNew uint8[size];
		Size = size;
	}

	FORCEINLINE void Release()
	{
		HcDelete[] Data;
		Data = nullptr;
		Size = 0;
	}
};

} // namespace HC