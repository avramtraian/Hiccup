// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"

#include <new>

// Whether or not the Memory Tracking Tool is included in the binaries (and, thus, can be used).
#define HC_ENABLE_MEMORY_TRACKING           1

#if HC_ENABLE_MEMORY_TRACKING
	// New operator that provides memory tracking functionality. This should be used for most allocations.
	#define hc_new                          new(HC_FILE, HC_FUNCTION_SIG, HC_LINE)

	// Delete operator that provides memory tracking functionality. This should be used for most deallocations.
	#define hc_delete                       delete

	// Helper macro, used for automatically filling the 'allocate_tagged' tracking information.
	#define allocate_tagged_i(BYTES_COUNT)  allocate_tagged((BYTES_COUNT), HC_FILE, HC_FUNCTION_SIG, HC_LINE)
#else
	// If the Memory Tracking Tool is not present in the binaries, there is no gain/need to use the tagged functions.
	#define hc_new                          new
	#define hc_delete                       delete
	#define allocate_tagged_i(BYTES_COUNT)  Allocate(BYTES_COUNT)
#endif // HC_ENABLE_MEMORY_TRACKING

namespace HC
{

/**
 *----------------------------------------------------------------
 * Memory System Description.
 *----------------------------------------------------------------
 */
struct MemoryDescription
{
	// Whether or not to initialize the tracker. This flag is ignored when
	//   'HC_ENABLE_MEMORY_TRACKING' is set to 0.
	bool should_initialize_tracker;
};

/**
 *----------------------------------------------------------------
 * Hiccup Memory System.
 *----------------------------------------------------------------
 * This class holds all the functionality that the Hiccup Memory API provides.
 */
class Memory
{
public:
	static bool initialize(const MemoryDescription& description);
	static void shutdown();

public:
	/**
	 * Copies a block of memory.
	 * Both detination and source memory buffers must be at least bytes_count long.
	 * 
	 * @param destination Where the memory will be copied to.
	 * @param source Where the memory will be copied from.
	 * @param bytes_count The number of bytes to copy.
	 */
	HC_API static void copy(void* detination, const void* source, size_t bytes_count);

	/**
	 * Sets a block of memory to a given value.
	 * The destination memory buffer must be at least bytes_count long.
	 *
	 * @param destination The address of the memory block to set.
	 * @param value The value for each byte.
	 * @param bytes_count The number of bytes to set.
	 */
	HC_API static void set(void* detination, uint8_t value, size_t bytes_count);

	/**
	 * Zeros a block of memory.
	 * The destination memory buffer must be at least bytes_count long.
	 *
	 * @param destination The address of the memory block to zero.
	 * @param bytes_count The number of bytes to zero.
	 */
	HC_API static void zero(void* detination, size_t bytes_count);

public:
	/**
	 * Allocates a block of memory.
	 * The memory block allocated must only be freed by the 'FreeRaw' function.
	 * This function doesn't use any kind of memory tracking. If you want that functionality,
	 *   use 'Allocate' or 'AllocateTagged'.
	 * 
	 * @param bytes_count The number of bytes the memory block will have.
	 * 
	 * @return The address of the allocated memory block.
	 */
	HC_API static void* allocate_raw(size_t bytes_count);

	/**
	 * Allocates a block of memory.
	 * The memory block allocated must only be freed by the 'Free' function.
	 * 
	 * @param bytes_count The number of bytes the memory block will have.
	 *
	 * @return The address of the allocated memory block.
	 */
	HC_API static void* allocate(size_t bytes_count);

	/**
	 * Allocates a block of memory.
	 * The memory block allocated must only be freed by the 'Free' function.
	 * This function does a more extensive (and thus more performance expensive) form of
	 *   memory tracking. All the allocations performed by this function are registered
	 *   and stored along the file where the allocations happen, the function signature and the line number.
	 *
	 * @param bytes_count The number of bytes the memory block will have.
	 * @param filename The file where the allocation was requested/performed.
	 * @param function_sig The function where the allocation was requested/performed.
	 * @param line_number The line number where the allocation was requested/performed.
	 *
	 * @return The address of the allocated memory block.
	*/
	HC_API static void* allocate_tagged(size_t bytes_count, const char* filename, const char* function_sig, uint32_t line_number);

	/**
	 * Frees a memory block.
	 * Can only free blocks allocated by the 'AllocateRaw' function.
	 * 
	 * @param memory_block The address of the memory block to free.
	*/
	HC_API static void free_raw(void* memory_block);

	/**
	 * Frees a memory block.
	 * It can't free blocks allocated by the 'AllocateRaw' function, due to memory tracking.
	 *   Use 'FreeRaw' instead.
	 *
	 * @param memory_block The address of the memory block to free.
	 */
	HC_API static void free(void* memory_block);

#if HC_ENABLE_MEMORY_TRACKING

public:
	/**
	 *----------------------------------------------------------------------
	 * Memory Tracking/Debugging Tool.
	 *----------------------------------------------------------------------
	 * This class holds all the functionality that the Hiccup Memory Debugger provides.
	 * This tool has one chance to be initialized, during the Memory System initialization.
	 *   Also, if 'HC_ENABLE_MEMORY_TRACKING' is defined as 0, this system will be completely
	 *   removed from the binaries.
	 * This tool can only be used IF it was initialized. This can be checked by using 'IsActive'.
	 */
	class Tracker
	{
	public:
		/**
		 * Checks if the Memory Tracking Tools are available.
		 * 
		 * @return True if the system is active; False otherwise.
		 */
		HC_API static bool is_active();

	public:
		/** @return The total number of bytes allocated (from the global heap) during the application's lifetime. */
		HC_API static size_t get_total_allocated();

		/** @return The total number of allocations (from the global heap, of any size) during the application's lifetime. */
		HC_API static size_t get_total_allocations_count();

		/** @return The total number of bytes deallocated (from the global heap) during the application's lifetime. */
		HC_API static size_t get_total_deallocated();

		/** @return The total number of deallocations (from the global heap, of any size) during the application's lifetime. */
		HC_API static size_t get_total_deallocations_count();

		/** @return The number of bytes that are currently allocated (from the global heap). */
		HC_API static size_t get_current_allocated();

		/** @return The number of allocations (from the global heap, of any size) that are currently alive. */
		HC_API static size_t get_current_allocations_count();

		HC_API static void log_memory_usage();

	private:
		HC_API static void register_allocation(void* memory_block, size_t bytes_count);
		HC_API static void register_tagged_allocation(void* memory_block, size_t bytes_count, const char* filename, const char* function_sig, uint32_t line_number);

		HC_API static void register_deallocation(void* memory_block);

	private:
		static bool initialize();
		static void shutdown();

	private:
		friend class Memory;
	};

private:
	friend class Memory::Tracker;

#endif // HC_ENABLE_MEMORY_TRACKING
};

} // namespace HC

// New operator, providing only basic memory tracking functionality.
void* operator new(size_t bytes_count);

// New operator, providing full memory tracking functionality.
void* operator new(size_t bytes_count, const char* filename, const char* function_sig, uint32_t line_number);

// Delete operator.
void operator delete(void* memory_block) noexcept;

namespace HC
{

/**
 *----------------------------------------------------------------------
 * Hiccup Heap Allocator.
 *----------------------------------------------------------------------
 * Most common allocator you can use. It directly allocates the memory from the heap,
 *   using the 'Memory' calls. It provides the full range of memory tracking tools.
 * It is the default allocator used by the majority of the containers.
 */
class HeapAllocator
{
public:
	/** @see 'Memory::allocate_raw'. */
	ALWAYS_INLINE void* allocate_raw(size_t bytes_count)
	{
		return Memory::allocate(bytes_count);
	}

	/** @see 'Memory::allocate'. */
	ALWAYS_INLINE void* allocate(size_t bytes_count)
	{
		return Memory::allocate(bytes_count);
	}

	/** @see 'Memory::allocate_tagged'. */
	ALWAYS_INLINE void* allocate_tagged(size_t bytes_count, const char* filename, const char* function_sig, uint32_t line_number)
	{
		return Memory::allocate_tagged(bytes_count, filename, function_sig, line_number);
	}

	/** @see 'Memory::free_raw'. */
	ALWAYS_INLINE void free_raw(void* memory_block, size_t bytes_count)
	{
		Memory::free_raw(memory_block);
	}

	/** @see 'Memory::free'. */
	ALWAYS_INLINE void free(void* memory_block, size_t bytes_count)
	{
		Memory::free(memory_block);
	}

public:
	/** @returns Always true, because the memory is allocated directly from the global heap. */
	ALWAYS_INLINE constexpr bool operator==(const HeapAllocator&) const { return true; }
};

/**
 *----------------------------------------------------------------------
 * Hiccup Untracked Allocator.
 *----------------------------------------------------------------------
 * The most basic allocator you can use. It directly allocates the memory from the heap,
 *   using the 'Memory' calls, without performing any kind of memory tracking/debugging.
 */
class UntrackedAllocator
{
public:
	/** @see 'Memory::allocate_raw'. */
	ALWAYS_INLINE void* allocate_raw(size_t bytes_count)
	{
		return Memory::allocate_raw(bytes_count);
	}

	/** @see 'Memory::allocate'. */
	ALWAYS_INLINE void* allocate(size_t bytes_count)
	{
		return Memory::allocate_raw(bytes_count);
	}

	/** @see 'Memory::allocate_tagged'. */
	ALWAYS_INLINE void* allocate_tagged(size_t bytes_count, const char* filename, const char* function_sig, uint32_t line_number)
	{
		return Memory::allocate_raw(bytes_count);
	}

	/** @see 'Memory::free_raw'. */
	ALWAYS_INLINE void free_raw(void* memory_block, size_t bytes_count)
	{
		Memory::free_raw(memory_block);
	}

	/** @see 'Memory::free'. */
	ALWAYS_INLINE void free(void* memory_block, size_t bytes_count)
	{
		Memory::free_raw(memory_block);
	}

public:
	/** @returns Always true, because the memory is allocated directly from the global heap. */
	ALWAYS_INLINE constexpr bool operator==(const UntrackedAllocator&) const { return true; }
};

} // namespace HC