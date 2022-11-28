// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include <new>

// Whether or not the Memory Tracking Tool is included in the binaries (and, thus, can be used).
#define HC_ENABLE_MEMORY_TRACKING           1

#if HC_ENABLE_MEMORY_TRACKING
	// New operator that provides memory tracking functionality. This should be used for most allocations.
	#define HcNew                           new(HC_FILE, HC_FUNCTION_SIG, HC_LINE)

	// Delete operator that provides memory tracking functionality. This should be used for most deallocations.
	#define HcDelete                        delete

	// Helper macro, used for automatically filling the 'AllocateTagged' tracking information.
	#define AllocateTaggedI(BYTES_COUNT)    AllocateTagged((BYTES_COUNT), HC_FILE, HC_FUNCTION_SIG, HC_LINE)
#else
	// If the Memory Tracking Tool is not present in the binaries, there is no gain/need to use the tagged functions.
	#define HcNew                           new
	#define HcDelete                        delete
	#define AllocateTaggedI(BYTES_COUNT)    Allocate(BYTES_COUNT)
#endif // HC_ENABLE_MEMORY_TRACKING

namespace HC
{

/**
 *----------------------------------------------------------------
 * Memory System Specification.
 *----------------------------------------------------------------
*/
struct MemorySpecification
{
	// Whether or not to initialize the tracker. This flag is ignored when
	//   'HC_ENABLE_MEMORY_TRACKING' is set to 0.
	bool ShouldInitializeTracker;
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
	static bool Initialize(const MemorySpecification& specification);
	static void Shutdown();

public:
	/**
	 * Copies a block of memory.
	 * Both detination and source memory buffers must be at least bytesCount long.
	 * 
	 * @param destination Where the memory will be copied to.
	 * @param source Where the memory will be copied from.
	 * @param bytesCount The number of bytes to copy.
	 */
	// TODO(Traian): Maybe inline this function?
	HC_API static void Copy(void* detination, const void* source, usize bytesCount);

	/**
	 * Sets a block of memory to a given value.
	 * The destination memory buffer must be at least bytesCount long.
	 *
	 * @param destination The address of the memory block to set.
	 * @param value The value for each byte.
	 * @param bytesCount The number of bytes to set.
	 */
	 // TODO(Traian): Maybe inline this function?
	HC_API static void Set(void* detination, uint8 value, usize bytesCount);

	/**
	 * Zeros a block of memory.
	 * The destination memory buffer must be at least bytesCount long.
	 *
	 * @param destination The address of the memory block to zero.
	 * @param bytesCount The number of bytes to zero.
	 */
	 // TODO(Traian): Maybe inline this function?
	HC_API static void Zero(void* detination, usize bytesCount);

public:
	/**
	 * Allocates a block of memory.
	 * The memory block allocated must only be freed by the 'FreeRaw' function.
	 * This function doesn't use any kind of memory tracking. If you want that functionality,
	 *   use 'Allocate' or 'AllocateTagged'.
	 * 
	 * @param bytesCount The number of bytes the memory block will have.
	 * 
	 * @return The address of the allocated memory block.
	*/
	HC_API static void* AllocateRaw(usize bytesCount);

	/**
	 * Allocates a block of memory.
	 * The memory block allocated must only be freed by the 'Free' function.
	 * 
	 * @param bytesCount The number of bytes the memory block will have.
	 *
	 * @return The address of the allocated memory block.
	*/
	HC_API static void* Allocate(usize bytesCount);

	/**
	 * Allocates a block of memory.
	 * The memory block allocated must only be freed by the 'Free' function.
	 * This function does a more extensive (and thus more performance expensive) form of
	 *   memory tracking. All the allocations performed by this function are registered
	 *   and stored along the file where the allocations happen, the function signature and the line number.
	 *
	 * @param bytesCount The number of bytes the memory block will have.
	 * @param fileName The file where the allocation was requested/performed.
	 * @param functionName The function where the allocation was requested/performed.
	 * @param lineNumber The line number where the allocation was requested/performed.
	 *
	 * @return The address of the allocated memory block.
	*/
	HC_API static void* AllocateTagged(usize bytesCount, const char* fileName, const char* functionName, HC::uint32 lineNumber);

	/**
	 * Frees a memory block.
	 * Can only free blocks allocated by the 'AllocateRaw' function.
	 * 
	 * @param memoryBlock The address of the memory block to free.
	*/
	HC_API static void FreeRaw(void* memoryBlock);

	/**
	 * Frees a memory block.
	 * It can't free blocks allocated by the 'AllocateRaw' function, due to memory tracking.
	 *   Use 'FreeRaw' instead.
	 *
	 * @param memoryBlock The address of the memory block to free.
	*/
	HC_API static void Free(void* memoryBlock);

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
		HC_API static bool IsActive();

	public:
		/** @return The total number of bytes allocated (from the global heap) during the application's lifetime. */
		HC_API static usize GetTotalAllocated();

		/** @return The total number of allocations (from the global heap, of any size) during the application's lifetime. */
		HC_API static usize GetTotalAllocationsCount();

		/** @return The total number of bytes deallocated (from the global heap) during the application's lifetime. */
		HC_API static usize GetTotalDeallocated();

		/** @return The total number of deallocations (from the global heap, of any size) during the application's lifetime. */
		HC_API static usize GetTotalDeallocationsCount();

		/** @return The number of bytes that are currently allocated (from the global heap). */
		HC_API static usize GetCurrentAllocated();

		/** @return The number of allocations (from the global heap, of any size) that are currently alive. */
		HC_API static usize GetCurrentAllocationsCount();

		HC_API static void LogMemoryUsage();

	private:
		HC_API static void RegisterAllocation(void* memoryBlock, usize bytesCount);
		HC_API static void RegisterTaggedAllocation(void* memoryBlock, usize bytesCount, const char* fileName, const char* functionName, HC::uint32 lineNumber);

		HC_API static void RegisterDeallocation(void* memoryBlock);

	private:
		static bool Initialize();
		static void Shutdown();

	private:
		friend class Memory;
	};

private:
	friend class Memory::Tracker;

#endif // HC_ENABLE_MEMORY_TRACKING
};

} // namespace HC

// New operator, providing only basic memory tracking functionality.
void* operator new(size_t bytesCount);

// New operator, providing full memory tracking functionality.
void* operator new(size_t bytesCount, const char* fileName, const char* functionName, HC::uint32 lineNumber);

// Delete operator.
void operator delete(void* memoryBlock) noexcept;

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
	/** @see 'Memory::AllocateRaw'. */
	FORCEINLINE void* AllocateRaw(usize bytesCount)
	{
		return Memory::Allocate(bytesCount);
	}

	/** @see 'Memory::Allocate'. */
	FORCEINLINE void* Allocate(usize bytesCount)
	{
		return Memory::Allocate(bytesCount);
	}

	/** @see 'Memory::AllocateTagged'. */
	FORCEINLINE void* AllocateTagged(usize bytesCount, const char* fileName, const char* functionName, uint32 lineNumber)
	{
		return Memory::AllocateTagged(bytesCount, fileName, functionName, lineNumber);
	}

	/** @see 'Memory::FreeRaw'. */
	FORCEINLINE void FreeRaw(void* memoryBlock, usize bytesCount)
	{
		Memory::FreeRaw(memoryBlock);
	}

	/** @see 'Memory::Free'. */
	FORCEINLINE void Free(void* memoryBlock, usize bytesCount)
	{
		Memory::Free(memoryBlock);
	}

public:
	/** @returns Always true, because the memory is allocated directly from the global heap. */
	FORCEINLINE constexpr bool operator==(const HeapAllocator&) const { return true; }
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
	/** @see 'Memory::AllocateRaw'. */
	FORCEINLINE void* AllocateRaw(usize bytesCount)
	{
		return Memory::AllocateRaw(bytesCount);
	}

	/** @see 'Memory::AllocateRaw'. */
	FORCEINLINE void* Allocate(usize bytesCount)
	{
		return Memory::AllocateRaw(bytesCount);
	}

	/** @see 'Memory::AllocateRaw'. */
	FORCEINLINE void* AllocateTagged(usize bytesCount, const char* fileName, const char* functionName, uint32 lineNumber)
	{
		return Memory::AllocateRaw(bytesCount);
	}

	/** @see 'Memory::FreeRaw'. */
	FORCEINLINE void FreeRaw(void* memoryBlock, usize bytesCount)
	{
		Memory::FreeRaw(memoryBlock);
	}

	/** @see 'Memory::FreeRaw'. */
	FORCEINLINE void Free(void* memoryBlock, usize bytesCount)
	{
		Memory::FreeRaw(memoryBlock);
	}

public:
	/** @returns Always true, because the memory is allocated directly from the global heap. */
	FORCEINLINE constexpr bool operator==(const UntrackedAllocator&) const { return true; }
};

} // namespace HC