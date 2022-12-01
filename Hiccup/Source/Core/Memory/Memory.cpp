// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Memory.h"

#include "Core/Platform/Platform.h"
#include "Core/Containers/HashTable.h"

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
	s_MemoryData = (MemoryData*)Platform::AllocateMemory(sizeof(MemoryData));
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
	Platform::FreeMemory(s_MemoryData);
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

	return Platform::AllocateMemory(bytesCount);
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
	Platform::FreeMemory(memoryBlock);
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

template<typename KeyType, typename ValueType>
using UntrackedHashTable = HashTable<KeyType, ValueType, UntrackedAllocator>;

struct AllocationInfo
{
	usize       BytesCount;
	const char* FileName;
	const char* FunctionName;
	uint32      LineNumber;
};

struct MemoryTrackerData
{
	usize Allocated             = 0;
	usize AllocationsCount      = 0;
	usize Deallocated           = 0;
	usize DeallocationsCount    = 0;

	UntrackedHashTable<void*, AllocationInfo> AllocationsTable;
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

void Memory::Tracker::LogMemoryUsage()
{
	s_TrackerData->AllocationsTable.ForEach([](void* memoryBlock, const AllocationInfo& allocation) -> bool
		{
			HC_LOG_DEBUG("Allocation [%p]:", memoryBlock);
			HC_LOG_DEBUG("    BytesCount:   %u", allocation.BytesCount);
			HC_LOG_DEBUG("    FileName:     %s", allocation.FileName);
			HC_LOG_DEBUG("    FunctionName: %s", allocation.FunctionName);
			HC_LOG_DEBUG("    LineNumber:   %u", allocation.LineNumber);
			return true;
		});
}

void Memory::Tracker::RegisterAllocation(void* memoryBlock, usize bytesCount)
{
	s_TrackerData->Allocated += bytesCount;
	s_TrackerData->AllocationsCount++;
}

void Memory::Tracker::RegisterTaggedAllocation(void* memoryBlock, usize bytesCount, const char* fileName, const char* functionName, uint32 lineNumber)
{
	AllocationInfo allocation = {};
	allocation.BytesCount = bytesCount;
	allocation.FileName = fileName;
	allocation.FunctionName = functionName;
	allocation.LineNumber = lineNumber;

	s_TrackerData->Allocated += bytesCount;
	s_TrackerData->AllocationsCount++;

	s_TrackerData->AllocationsTable.Insert(memoryBlock, Types::Move(allocation));
}

void Memory::Tracker::RegisterDeallocation(void* memoryBlock)
{
	const usize allocationIndex = s_TrackerData->AllocationsTable.FindExistingIndex(memoryBlock);
	const AllocationInfo& allocation = s_TrackerData->AllocationsTable.AtIndex(allocationIndex);

	s_TrackerData->Deallocated += allocation.BytesCount;
	s_TrackerData->DeallocationsCount++;

	s_TrackerData->AllocationsTable.RemoveIndex(allocationIndex);
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