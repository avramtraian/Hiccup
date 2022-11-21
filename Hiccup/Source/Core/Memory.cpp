// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Memory.h"

#include "Platform/Platform.h"

#include <cstring>

namespace HC
{

struct MemoryData
{
	MemorySpecification Specification;
};
internal MemoryData* s_MemoryData = nullptr;

bool Memory::Initialize(const MemorySpecification& specification)
{
	s_MemoryData = (MemoryData*)Platform::Allocate(sizeof(MemoryData));
	new (s_MemoryData) MemoryData();

	s_MemoryData->Specification = specification;

#if HC_ENABLE_MEMORY_TRACKING
	if (s_MemoryData->Specification.ShouldInitializeTracker)
	{
		return Tracker::Initialize();
	}
#endif // HC_ENABLE_MEMORY_TRACKING

	return true;
}

void Memory::Shutdown()
{
#if HC_ENABLE_MEMORY_TRACKING
	if (Tracker::IsActive())
	{
		Tracker::Shutdown();
	}
#endif // HC_ENABLE_MEMORY_TRACKING

	s_MemoryData->~MemoryData();
	Platform::Free(s_MemoryData);
	s_MemoryData = nullptr;
}

void Memory::Copy(void* detination, const void* source, usize bytesCount)
{
	std::memcpy(detination, source, bytesCount);
}

void Memory::Set(void* detination, uint8 value, usize bytesCount)
{
	std::memset(detination, (int)value, bytesCount);
}

void Memory::Zero(void* detination, usize bytesCount)
{
	Memory::Set(detination, 0, bytesCount);
}

void* Memory::AllocateRaw(usize bytesCount)
{
	if (bytesCount == 0)
	{
		return nullptr;
	}

	return Platform::Allocate(bytesCount);
}

void* Memory::Allocate(usize bytesCount)
{
	if (bytesCount == 0)
	{
		return nullptr;
	}

	void* memoryBlock = AllocateRaw(bytesCount);
#if HC_ENABLE_MEMORY_TRACKING
	if (Tracker::IsActive())
	{
		Tracker::RegisterAllocation(memoryBlock, bytesCount);
	}
#endif // HC_ENABLE_MEMORY_TRACKING
	return memoryBlock;
}

void* Memory::AllocateTagged(usize bytesCount, const char* fileName, const char* functionName, HC::uint32 lineNumber)
{
	if (bytesCount == 0)
	{
		return nullptr;
	}

	void* memoryBlock = AllocateRaw(bytesCount);
#if HC_ENABLE_MEMORY_TRACKING
	if (Tracker::IsActive())
	{
		Tracker::RegisterTaggedAllocation(memoryBlock, bytesCount, fileName, functionName, lineNumber);
	}
#endif // HC_ENABLE_MEMORY_TRACKING
	return memoryBlock;
}

void Memory::FreeRaw(void* memoryBlock)
{
	Platform::Free(memoryBlock);
}

void Memory::Free(void* memoryBlock)
{
	if (!memoryBlock)
	{
		return;
	}

#if HC_ENABLE_MEMORY_TRACKING
	if (Tracker::IsActive())
	{
		Tracker::RegisterDeallocation(memoryBlock);
	}
#endif // HC_ENABLE_MEMORY_TRACKING
	FreeRaw(memoryBlock);
}

#if HC_ENABLE_MEMORY_TRACKING
struct MemoryTrackerData
{
	usize Allocated             = 0;
	usize AllocationsCount      = 0;
	usize Deallocated           = 0;
	usize DeallocationsCount    = 0;
};
internal MemoryTrackerData* s_TrackerData = nullptr;

bool Memory::Tracker::Initialize()
{
	s_TrackerData = (MemoryTrackerData*)Memory::AllocateRaw(sizeof(MemoryTrackerData));
	if (!s_TrackerData)
	{
		return false;
	}

	new (s_TrackerData) MemoryTrackerData();

	return true;
}

void Memory::Tracker::Shutdown()
{
	s_TrackerData->~MemoryTrackerData();
	Memory::FreeRaw(s_TrackerData);
	s_TrackerData = nullptr;
}

bool Memory::Tracker::IsActive()
{
	return s_TrackerData != nullptr;
}

usize Memory::Tracker::GetTotalAllocated()
{
	return s_TrackerData->Allocated;
}

usize Memory::Tracker::GetTotalAllocationsCount()
{
	return s_TrackerData->AllocationsCount;
}

usize Memory::Tracker::GetTotalDeallocated()
{
	return s_TrackerData->Deallocated;
}

usize Memory::Tracker::GetTotalDeallocationsCount()
{
	return s_TrackerData->DeallocationsCount;
}

usize Memory::Tracker::GetCurrentAllocated()
{
	return s_TrackerData->Allocated - s_TrackerData->Deallocated;
}

usize Memory::Tracker::GetCurrentAllocationsCount()
{
	return s_TrackerData->AllocationsCount - s_TrackerData->DeallocationsCount;
}

void Memory::Tracker::RegisterAllocation(void* memoryBlock, usize bytesCount)
{
	s_TrackerData->Allocated += bytesCount;
	s_TrackerData->AllocationsCount++;
}

void Memory::Tracker::RegisterTaggedAllocation(void* memoryBlock, usize bytesCount, const char* fileName, const char* functionName, HC::uint32 lineNumber)
{
	s_TrackerData->Allocated += bytesCount;
	s_TrackerData->AllocationsCount++;
}

void Memory::Tracker::RegisterDeallocation(void* memoryBlock)
{
	// TODO(Traian): Retrieve this information from the allocations table.
	s_TrackerData->Deallocated += 0;
	s_TrackerData->DeallocationsCount++;
}
#endif // HC_ENABLE_MEMORY_TRACKING

} // namespace HC

void* operator new(size_t bytesCount)
{
	return HC::Memory::Allocate(bytesCount);
}

void* operator new(size_t bytesCount, const char* fileName, const char* functionName, HC::uint32 lineNumber)
{
	return HC::Memory::AllocateTagged(bytesCount, fileName, functionName, lineNumber);
}

void operator delete(void* memoryBlock)
{
	HC::Memory::Free(memoryBlock);
}