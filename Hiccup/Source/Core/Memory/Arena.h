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
	HC_NON_COPIABLE(LinearMemoryArena)
	HC_NON_MOVABLE(LinearMemoryArena)

	// Default constructor.
	ALWAYS_INLINE LinearMemoryArena()
		: m_buffer()
		, m_allocated(0)
	{}

	/**
	 * Constructs an arena that can store 'size' bytes.
	 * 
	 * @param size The number of bytes the arena can store.
	 */
	ALWAYS_INLINE LinearMemoryArena(usize size)
		: m_buffer(size)
		, m_allocated(0)
	{}

	// Destructor.
	ALWAYS_INLINE ~LinearMemoryArena()
	{
		release_memory();
	}

public:
	/** @return Pointer to the arena's memory block. */
	ALWAYS_INLINE uint8* data() const { return m_buffer.data; }

	/** @return The number of bytes the arena can store. */
	ALWAYS_INLINE usize size() const { return m_buffer.size; }

	/** @return The number of currently allocated bytes. */
	ALWAYS_INLINE usize allocated() const { return m_allocated; }

	ALWAYS_INLINE bool is_valid() const { return (m_buffer.data != nullptr); }

public:
	/** @return True if the arena can store the given number of bytes; False otherwise. */
	ALWAYS_INLINE bool is_able_to_store(usize bytes_count)
	{
		return (m_allocated + bytes_count <= m_buffer.size);
	}

	/** @return True if the arena can store an instance of the given type; False otherwise. */
	template<typename T>
	ALWAYS_INLINE bool is_able_to_store_type()
	{
		return is_able_to_store(sizeof(T));
	}

	/** @return True if the arena can store an array of the given type; False otherwise. */
	template<typename T>
	ALWAYS_INLINE bool is_able_to_store_array(usize count)
	{
		return is_able_to_store(count * sizeof(T));
	}

public:
	/**
	 * Allocates a block of memory from the arena.
	 * If the arena doesn't have enough space for it, nullptr is returned.
	 * 
	 * @param bytes_count The size (in bytes) of the requested memory block.
	 * 
	 * @return Pointer to the allocated memory block, or nullptr on failure.
	 */
	uint8* allocate(usize bytes_count)
	{
		if ((bytes_count == 0) || !is_able_to_store(bytes_count))
		{
			return nullptr;
		}

		uint8* memory = m_buffer.data + m_allocated;
		m_allocated += bytes_count;
		return memory;
	}

	/**
	 * Allocates a block of memory from the arena, casting the result to the
	 *   given type.
	 * Same behavior as the 'allocate'.
	 * 
	 * @param bytes_count The size (in bytes) of the requested memory block.
	 * 
	 * @return Pointer to the allocated memory block, casted to the given type.
	 */
	template<typename T>
	ALWAYS_INLINE T* allocate_as(usize bytes_count)
	{
		return (T*)allocate(bytes_count);
	}

	/**
	 * Allocates enough memory for the provided type.
	 * Same behavior as the 'allocate'.
	 * 
	 * @return Pointer to the allocated memory block, casted to the given type.
	 */
	template<typename T>
	ALWAYS_INLINE T* allocate_type()
	{
		return (T*)allocate(sizeof(T));
	}

	/**
	 * Allocates enough memory for an array of the provided type.
	 * Same behavior as 'allocate'.
	 * 
	 * @param count The number of elements of the array.
	 * 
	 * @retirm Pointer the allocated memory block, casted to the given type.
	 */
	template<typename T>
	ALWAYS_INLINE T* allocate_array(usize count)
	{
		return (T*)allocate(count * sizeof(T));
	}
	
	/**
	 * Resets the arena. The arena's memory buffer is not released from memory,
	 *   but rather the allocated number of bytes is set to 0.
	 */
	void reset()
	{
		m_allocated = 0;
	}

public:
	/**
	 * Copies a memory arena.
	 * 
	 * @param source The memory arena to copy.
	 * @param destination Pointer to the destination memory arena. This must be
	 *   an invalid memory arena.
	 */
	static void copy(const LinearMemoryArena& source, LinearMemoryArena* destination)
	{
		HC_ASSERT(!destination->is_valid()); // Destination is a valid memory arena!

		destination->m_buffer = Buffer::copy(source.m_buffer);
		destination->m_allocated = source.m_allocated;
	}

public:
	/**
	 * Allocates the arena's memory buffer from the heap.
	 * 
	 * @param bytesCount The number of bytes the arena can store.
	 */
	void allocate_memory(usize bytes_count)
	{
		reset();
		m_buffer.allocate(bytes_count);
	}

	/**
	 * Releases the arena's memory buffer.
	 * This function must be called manually when the arena is no longer required.
	 */
	void release_memory()
	{
		reset();
		m_buffer.release();
	}

private:
	// The arena memory buffer.
	Buffer m_buffer;

	// The number of bytes currently allocated.
	usize m_allocated;
};

/**
 *----------------------------------------------------------------------
 * Hiccup Stack Memory Arena.
 *----------------------------------------------------------------------
 * This memory arena works exactly like a stack. The performance is
 *   equivalent with 'LinearMemoryArena', with the ability to free
 *   allocations.
 * Deallocations must happen in the order opposite to their corresponding allocations.
 */
struct HC_API StackMemoryArena
{
public:
	HC_NON_COPIABLE(StackMemoryArena)
	HC_NON_MOVABLE(StackMemoryArena)

	// Default constructor.
	ALWAYS_INLINE StackMemoryArena()
		: m_buffer()
		, m_allocated(0)
	{}

	/**
	 * Constructs an arena that can store 'size' bytes.
	 * 
	 * @param size The number of bytes the arena can store.
	 */
	ALWAYS_INLINE StackMemoryArena(usize size)
		: m_buffer(size)
		, m_allocated(0)
	{}

	// Destructor.
	ALWAYS_INLINE ~StackMemoryArena()
	{
		release_memory();
	}

public:
	/** @return Pointer to the arena's memory block. */
	ALWAYS_INLINE uint8* data() const { return m_buffer.data; }

	/** @return The number of bytes the arena can store. */
	ALWAYS_INLINE usize size() const { return m_buffer.size; }

	/** @return The number of currently allocated bytes. */
	ALWAYS_INLINE usize allocated() const { return m_allocated; }

	ALWAYS_INLINE bool is_valid() const { return (m_buffer.data != nullptr); }

public:
	/** @return True if the arena can store the given number of bytes; False otherwise. */
	ALWAYS_INLINE bool is_able_to_store(usize bytes_count)
	{
		return (m_allocated + bytes_count <= m_buffer.size);
	}

	/** @return True if the arena can store an instance of the given type; False otherwise. */
	template<typename T>
	ALWAYS_INLINE bool is_able_to_store_type()
	{
		return is_able_to_store(sizeof(T));
	}

	/** @return True if the arena can store an array of the given type; False otherwise. */
	template<typename T>
	ALWAYS_INLINE bool is_able_to_store_array(usize count)
	{
		return is_able_to_store(count * sizeof(T));
	}

public:
	uint8* push(usize bytes_count)
	{
		if ((bytes_count == 0) || !is_able_to_store(bytes_count))
		{
			return nullptr;
		}

		uint8* memory = m_buffer.data + m_allocated;
		m_allocated += bytes_count;
		return memory;
	}

	template<typename T>
	ALWAYS_INLINE T* push_as(usize bytes_count)
	{
		return (T*)push(bytes_count);
	}

	template<typename T>
	ALWAYS_INLINE T* push_type()
	{
		return (T*)push(sizeof(T));
	}

	template<typename T>
	ALWAYS_INLINE T* push_array(usize count)
	{
		return (T*)push(count * sizeof(T));
	}

	void pop(usize bytes_count)
	{
		HC_ASSERT(m_allocated >= bytes_count); // Trying to pop too much from the stack!
		m_allocated -= bytes_count;
	}

	template<typename T>
	ALWAYS_INLINE void pop_type()
	{
		pop(sizeof(T));
	}

	template<typename T>
	ALWAYS_INLINE void pop_array(usize count)
	{
		pop(count * sizeof(T));
	}

	void pop(void* memory_address)
	{
		uint8* memory = (uint8*)memory_address;

		// Memory address doesn't belong to the stack!
		HC_ASSERT(m_buffer.data <= memory_address && memory_address < m_buffer.data + m_allocated);

		m_allocated = memory - m_buffer.data;
	}

	/**
	 * Resets the arena. The arena's memory buffer is not released from memory,
	 *   but rather the allocated number of bytes is set to 0.
	 */
	void reset()
	{
		m_allocated = 0;
	}

public:
	/**
	 * Copies a memory arena.
	 * 
	 * @param source The memory arena to copy.
	 * @param destination Pointer to the destination memory arena. This must be
	 *   an invalid memory arena.
	 */
	static void copy(const StackMemoryArena& source, StackMemoryArena* destination)
	{
		HC_ASSERT(!destination->is_valid()); // Destination is a valid memory arena!

		destination->m_buffer = Buffer::copy(source.m_buffer);
		destination->m_allocated = source.m_allocated;
	}

public:
	/**
	 * Allocates the arena's memory buffer from the heap.
	 * 
	 * @param bytesCount The number of bytes the arena can store.
	 */
	void allocate_memory(usize bytes_count)
	{
		reset();
		m_buffer.allocate(bytes_count);
	}

	/**
	 * Releases the arena's memory buffer.
	 * This function must be called manually when the arena is no longer required.
	 */
	void release_memory()
	{
		reset();
		m_buffer.release();
	}

private:
	// The arena memory buffer.
	Buffer m_buffer;

	// The number of bytes currently allocated.
	usize m_allocated;
};

} // namespace HC