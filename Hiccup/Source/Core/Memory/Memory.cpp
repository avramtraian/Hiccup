// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Memory.h"

#include "Core/Platform/Platform.h"
#include "Core/Containers/HashTable.h"

#include <cstring>

namespace HC
{

struct MemoryData
{
	MemoryDescription description;
};
static_internal MemoryData* s_memory_data = nullptr;

bool Memory::initialize(const MemoryDescription& description)
{
	s_memory_data = (MemoryData*)Platform::allocate_memory(sizeof(MemoryData));
	new (s_memory_data) MemoryData();

	s_memory_data->description = description;

#if HC_ENABLE_MEMORY_TRACKING
	if (s_memory_data->description.should_initialize_tracker)
	{
		return Tracker::initialize();
	}
#endif // HC_ENABLE_MEMORY_TRACKING

	return true;
}

void Memory::shutdown()
{
#if HC_ENABLE_MEMORY_TRACKING
	if (Tracker::is_active())
	{
		Tracker::shutdown();
	}
#endif // HC_ENABLE_MEMORY_TRACKING

	s_memory_data->~MemoryData();
	Platform::free_memory(s_memory_data);
	s_memory_data = nullptr;
}

void Memory::copy(void* detination, const void* source, usize bytes_count)
{
	std::memcpy(detination, source, bytes_count);
}

void Memory::set(void* detination, uint8 value, usize bytes_count)
{
	std::memset(detination, (int)value, bytes_count);
}

void Memory::zero(void* detination, usize bytes_count)
{
	Memory::set(detination, 0, bytes_count);
}

void* Memory::allocate_raw(usize bytes_count)
{
	if (bytes_count == 0)
	{
		return nullptr;
	}

	return Platform::allocate_memory(bytes_count);
}

void* Memory::allocate(usize bytes_count)
{
	if (bytes_count == 0)
	{
		return nullptr;
	}

	void* memory_block = allocate_raw(bytes_count);
#if HC_ENABLE_MEMORY_TRACKING
	if (Tracker::is_active())
	{
		Tracker::register_allocation(memory_block, bytes_count);
	}
#endif // HC_ENABLE_MEMORY_TRACKING
	return memory_block;
}

void* Memory::allocate_tagged(usize bytes_count, const char* filename, const char* function_sig, HC::uint32 line_number)
{
	if (bytes_count == 0)
	{
		return nullptr;
	}

	void* memory_block = allocate_raw(bytes_count);
#if HC_ENABLE_MEMORY_TRACKING
	if (Tracker::is_active())
	{
		Tracker::register_tagged_allocation(memory_block, bytes_count, filename, function_sig, line_number);
	}
#endif // HC_ENABLE_MEMORY_TRACKING
	return memory_block;
}

void Memory::free_raw(void* memory_block)
{
	Platform::free_memory(memory_block);
}

void Memory::free(void* memory_block)
{
	if (!memory_block)
	{
		return;
	}

#if HC_ENABLE_MEMORY_TRACKING
	if (Tracker::is_active())
	{
		Tracker::register_deallocation(memory_block);
	}
#endif // HC_ENABLE_MEMORY_TRACKING
	free_raw(memory_block);
}

#if HC_ENABLE_MEMORY_TRACKING

template<typename KeyType, typename ValueType>
using UntrackedHashTable = HashTable<KeyType, ValueType, UntrackedAllocator>;

struct AllocationInfo
{
	usize       bytes_count;
	const char* filename;
	const char* function_sig;
	uint32      line_number;
};

struct MemoryTrackerData
{
	usize allocated             = 0;
	usize allocations_count     = 0;
	usize deallocated           = 0;
	usize deallocations_count   = 0;

	UntrackedHashTable<void*, AllocationInfo> allocations_table;
};
static_internal MemoryTrackerData* s_tracker_data = nullptr;

bool Memory::Tracker::initialize()
{
	s_tracker_data = (MemoryTrackerData*)Memory::allocate_raw(sizeof(MemoryTrackerData));
	if (!s_tracker_data)
	{
		return false;
	}

	new (s_tracker_data) MemoryTrackerData();

	return true;
}

void Memory::Tracker::shutdown()
{
	s_tracker_data->~MemoryTrackerData();
	Memory::free_raw(s_tracker_data);
	s_tracker_data = nullptr;
}

bool Memory::Tracker::is_active()
{
	return s_tracker_data != nullptr;
}

usize Memory::Tracker::get_total_allocated()
{
	return s_tracker_data->allocated;
}

usize Memory::Tracker::get_total_allocations_count()
{
	return s_tracker_data->allocations_count;
}

usize Memory::Tracker::get_total_deallocated()
{
	return s_tracker_data->deallocated;
}

usize Memory::Tracker::get_total_deallocations_count()
{
	return s_tracker_data->deallocations_count;
}

usize Memory::Tracker::get_current_allocated()
{
	return s_tracker_data->allocated - s_tracker_data->deallocated;
}

usize Memory::Tracker::get_current_allocations_count()
{
	return s_tracker_data->allocations_count - s_tracker_data->deallocations_count;
}

void Memory::Tracker::log_memory_usage()
{
	s_tracker_data->allocations_table.for_each([](void* memory_block, const AllocationInfo& allocation) -> bool
		{
			HC_LOG_DEBUG("Allocation [%p]:", memory_block);
			HC_LOG_DEBUG("    Bytes Count:        %u", allocation.bytes_count);
			HC_LOG_DEBUG("    Filename:           %s", allocation.filename);
			HC_LOG_DEBUG("    Function Signature: %s", allocation.function_sig);
			HC_LOG_DEBUG("    Line Number:        %u", allocation.line_number);
			return true;
		});
}

void Memory::Tracker::register_allocation(void* memory_block, usize bytes_count)
{
	s_tracker_data->allocated += bytes_count;
	s_tracker_data->allocations_count++;

	// TODO(Traian): Maybe have a separate table for allocation sizes?
	//   Seems a bit wasteful to have a full 'AllocationInfo' used for only a 'usize'.
	AllocationInfo allocation = {};
	allocation.bytes_count = bytes_count;

	s_tracker_data->allocations_table.insert(memory_block, Types::move(allocation));
}

void Memory::Tracker::register_tagged_allocation(void* memory_block, usize bytes_count, const char* filename, const char* function_sig, uint32 line_number)
{
	s_tracker_data->allocated += bytes_count;
	s_tracker_data->allocations_count++;

	AllocationInfo allocation = {};
	allocation.bytes_count = bytes_count;
	allocation.filename = filename;
	allocation.function_sig = function_sig;
	allocation.line_number = line_number;

	s_tracker_data->allocations_table.insert(memory_block, Types::move(allocation));
}

void Memory::Tracker::register_deallocation(void* memory_block)
{
	const usize allocationIndex = s_tracker_data->allocations_table.find_existing_index(memory_block);
	const AllocationInfo& allocation = s_tracker_data->allocations_table.at_index(allocationIndex);

	s_tracker_data->deallocated += allocation.bytes_count;
	s_tracker_data->deallocations_count++;

	s_tracker_data->allocations_table.remove_index(allocationIndex);
}
#endif // HC_ENABLE_MEMORY_TRACKING

} // namespace HC

void* operator new(size_t bytes_count)
{
	return HC::Memory::allocate(bytes_count);
}

void* operator new(size_t bytes_count, const char* filename, const char* function_sig, HC::uint32 line_number)
{
	return HC::Memory::allocate_tagged(bytes_count, filename, function_sig, line_number);
}

void operator delete(void* memory_block)
{
	HC::Memory::free(memory_block);
}