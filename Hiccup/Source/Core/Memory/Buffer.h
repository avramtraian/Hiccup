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
	uint8_t* data;

	// The number of bytes the buffer can store.
	size_t size;

public:
	// Default constructor.
	ALWAYS_INLINE Buffer()
		: data(nullptr)
		, size(0)
	{}

	/**
	 * Constructs a buffer that can store 'size' bytes.
	 *
	 * @param size The number of bytes the buffer can store.
	 */
	ALWAYS_INLINE Buffer(size_t in_size)
		: data(nullptr)
		, size(0)
	{
		allocate(in_size);
	}

	/**
	* It performs a shallow copy. For a deep copy, use
	*   'Buffer::copy'.
	*/
	ALWAYS_INLINE Buffer(const Buffer& other) = default;

public:
	/** @return Pointer to the buffer's memory block, casted to the given type. */
	template<typename T>
	ALWAYS_INLINE T* as() { return (T*)data; }

	/** @return Pointer to the buffer's memory block, casted to the given type. */
	template<typename T>
	ALWAYS_INLINE const T* as() const { return (const T*)data; }

	/**
	 * Copies a buffer, performing a deep copy.
	 * 
	 * @param source The buffer to copy.
	 * 
	 * @return The newly created buffer.
	 */
	ALWAYS_INLINE static Buffer copy(Buffer source)
	{
		Buffer destination = Buffer(source.size);
		Memory::copy(destination.data, source.data, source.size);
		return destination;
	}

public:
	/**
	 * Allocates the buffer's memory block.
	 * 
	 * @param size The number of bytes the buffer can store.
	 */
	ALWAYS_INLINE void allocate(size_t in_size)
	{
		if (data)
		{
			release();
		}

		data = hc_new uint8_t[in_size];
		size = in_size;
	}

	/**
	 * Releases the buffer's memory block.
	 */
	ALWAYS_INLINE void release()
	{
		hc_delete[] data;
		data = nullptr;
		size = 0;
	}
};

} // namespace HC