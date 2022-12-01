// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"

#include "Memory.h"
#include "Buffer.h"

namespace HC
{

/**
 *----------------------------------------------------------------------
 * Hiccup Linear Memory Arena.
 *----------------------------------------------------------------------
 * The most efficient memory arena you can use.
 * The linear arena works by storing the number of allocated bytes. Because
 *   allocations can't be released individually, the arena can always allocate
 *   right after the currently allocated portion. This means that no logic is
 *   required to find the space for the allocation.
 * As mentioned previous, its down side is that allocations can't be freed
 *   individually, but rather all at once. This can limit the arena's
 *   flexibility.
 */
struct HC_API LinearMemoryArena
{
public:
	// Default constructor.
	ALWAYS_INLINE LinearMemoryArena()
		: m_Buffer()
		, m_Allocated(0)
	{}

	/**
	 * Constructs an arena that can store 'size' bytes.
	 * 
	 * @param size The number of bytes the arena can store.
	 */
	ALWAYS_INLINE LinearMemoryArena(usize size)
		: m_Buffer(size)
		, m_Allocated(0)
	{}

	// Destructor.
	ALWAYS_INLINE ~LinearMemoryArena()
	{
		ReleaseMemory();
	}

	// Copy operations are not allowed.
	LinearMemoryArena(const LinearMemoryArena&) = delete;
	LinearMemoryArena& operator=(const LinearMemoryArena&) = delete;

public:
	/** @return Pointer to the arena's memory block. */
	ALWAYS_INLINE uint8* Data() const { return m_Buffer.Data; }

	/** @return The number of bytes the arena can store. */
	ALWAYS_INLINE usize Size() const { return m_Buffer.Size; }

	/** @return The number of currently allocated bytes. */
	ALWAYS_INLINE usize Allocated() const { return m_Allocated; }

	ALWAYS_INLINE bool IsValid() const { return (m_Buffer.Data != nullptr); }

public:
	/**
	 * Allocates a block of memory from the arena.
	 * If the arena doesn't have enough space for it, nullptr is returned.
	 * 
	 * @param bytesCount The size (in bytes) of the requested memory block.
	 * 
	 * @return Pointer to the allocated memory block, or nullptr on error.
	 */
	uint8* Allocate(usize bytesCount)
	{
		if ((bytesCount == 0) || !IsAbleToStore(bytesCount))
		{
			return nullptr;
		}

		uint8* memory = m_Buffer.Data + m_Allocated;
		m_Allocated += bytesCount;
		return memory;
	}

	template<typename T>
	ALWAYS_INLINE T* AllocateAs(usize bytesCount)
	{
		return (T*)Allocate(bytesCount);
	}

	/**
	 * Allocates enough memory for the provided type.
	 * Same behavior as the function above.
	 * 
	 * @return Pointer to the allocated memory block, casted to the given type.
	 */
	template<typename T>
	ALWAYS_INLINE T* AllocateType()
	{
		return (T*)Allocate(sizeof(T));
	}

	/**
	 * Allocates enough memory for an array of the provided type.
	 * Same behavior as the function above.
	 * 
	 * @param count The number of elements of the array.
	 * 
	 * @retirm Pointer the allocated memory block, casted to the given type.
	 */
	template<typename T>
	ALWAYS_INLINE T* AllocateArray(usize count)
	{
		return (T*)Allocate(count * sizeof(T));
	}
	
	/**
	 * Resets the arena. The arena's memory buffer is not released from memory,
	 *   but rather the allocated number of bytes is set to 0.
	 */
	void Reset()
	{
		m_Allocated = 0;
	}

	/** @return True if the arena can store the given number of bytes; False otherwise. */
	ALWAYS_INLINE bool IsAbleToStore(usize bytesCount)
	{
		return (m_Allocated + bytesCount <= m_Buffer.Size);
	}

	/** @return True if the arena can store an instance of the given type; False otherwise. */
	template<typename T>
	ALWAYS_INLINE bool IsAbleToStoreType()
	{
		return IsAbleToStore(sizeof(T));
	}

	/** @return True if the arena can store an array of the given type; False otherwise. */
	template<typename T>
	ALWAYS_INLINE bool IsAbleToStoreArray(usize count)
	{
		return IsAbleToStore(count * sizeof(T));
	}

public:
	/**
	 * Copies a memory arena.
	 * 
	 * @param source The memory arena to copy.
	 * @param destination Pointer to the destination memory arena. This must be
	 *   an invalid memory arena.
	 */
	static void Copy(const LinearMemoryArena& source, LinearMemoryArena* destination)
	{
		HC_ASSERT(!destination->IsValid()); // Destination is a valid memory arena!

		destination->m_Buffer = Buffer::Copy(source.m_Buffer);
		destination->m_Allocated = source.m_Allocated;
	}

public:
	/**
	 * Allocates the arena's memory buffer from the heap.
	 * 
	 * @param bytesCount The number of bytes the arena can store.
	 */
	void AllocateMemory(usize bytesCount)
	{
		Reset();
		m_Buffer.Allocate(bytesCount);
	}

	/**
	 * Releases the arena's memory buffer.
	 * This function must be called manually when the arena is no longer required.
	 */
	void ReleaseMemory()
	{
		Reset();
		m_Buffer.Release();
	}

private:
	// The arena memory buffer.
	Buffer m_Buffer;

	// The number of bytes currently allocated.
	usize m_Allocated;
};

} // namespace HC