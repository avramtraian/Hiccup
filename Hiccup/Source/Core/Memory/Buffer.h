// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"
#include "Memory.h"

namespace HC
{

/**
 *----------------------------------------------------------------------
 * Hiccup Memory Buffer.
 *----------------------------------------------------------------------
 * A zero-cost abstraction for an arbitrary memory buffer.
 * This is a light-weight object that can be created on the stack and
 *   be easily copied.
 * Its raw memory buffer's lifetime must be manually handled.
 */
struct HC_API Buffer
{
public:
	// Pointer the buffer's memory block.
	uint8* Data;

	// The number of bytes the buffer can store.
	usize Size;

public:
	// Default constructor.
	FORCEINLINE Buffer()
		: Data(nullptr)
		, Size(0)
	{}

	/**
	 * Constructs a buffer that can store 'size' bytes.
	 *
	 * @param size The number of bytes the buffer can store.
	 */
	FORCEINLINE Buffer(usize size)
		: Data(nullptr)
		, Size(0)
	{
		Allocate(size);
	}

	/**
	* Destructor. It performs a shallow copy. For a deep copy, use
	*   'Buffer::Copy'.
	*/
	FORCEINLINE Buffer(const Buffer& other) = default;

public:
	/** @return Pointer to the buffer's memory block, casted to the given type. */
	template<typename T>
	FORCEINLINE T* As() { return (T*)Data; }

	/** @return Pointer to the buffer's memory block, casted to the given type. */
	template<typename T>
	FORCEINLINE const T* As() const { return (const T*)Data; }

	/**
	 * Copies a buffer, performing a deep copy.
	 * 
	 * @param source The buffer to copy.
	 * 
	 * @return The newly created buffer.
	 */
	FORCEINLINE static Buffer Copy(Buffer source)
	{
		Buffer destination = Buffer(source.Size);
		Memory::Copy(destination.Data, source.Data, source.Size);
		return destination;
	}

public:
	/**
	 * Allocates the buffer's memory block.
	 * 
	 * @param size The number of bytes the buffer can store.
	 */
	FORCEINLINE void Allocate(usize size)
	{
		if (Data)
		{
			Release();
		}

		Data = HcNew uint8[size];
		Size = size;
	}

	/**
	 * Releases the buffer's memory block.
	 */
	FORCEINLINE void Release()
	{
		HcDelete[] Data;
		Data = nullptr;
		Size = 0;
	}
};

} // namespace HC