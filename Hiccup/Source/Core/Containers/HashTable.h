// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/Memory/Memory.h"

#include "Hash.h"
#include "Comparator.h"

namespace HC
{

/**
 *----------------------------------------------------------------
 * Hiccup Container - HashTable.
 *----------------------------------------------------------------
 */
template<typename KeyType, typename ValueType, typename AllocatorType = HeapAllocator, typename Hasher = DefaultHasher, typename Comparator = DefaultComparator>
class HashTable
{
public:
	// Structure holding the key-value pairs.
	struct KeyValue
	{
		KeyType     Key;
		ValueType   Value;
	};

	// The state that a bucket can have.
	enum class BucketState : uint8
	{
		Empty = 0x00, Deleted = 0xDD, Occupied = 0xFF
	};

	// The maximum allowed table load factor.
	static constexpr float64 MaxLoadFactor = 0.75;

	static constexpr usize EndOfTable = static_cast<usize>(-1);

public:
	/**
	 * Default constructor.
	 */
	HashTable();

	/**
	 * Copy constructor.
	 * 
	 * @param other The hash table to copy.
	 */
	HashTable(const HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>& other);

	/**
	 * Move constructor.
	 *
	 * @param other The hash table to move.
	 */
	HashTable(HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>&& other) noexcept;

	/**
	 * Destructor.
	 */
	~HashTable();

	/**
	 * Copy assignment operator.
	 * 
	 * @param other The hash table to copy.
	 * 
	 * @return Reference to this, after the copy.
	 */
	HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>& operator=(const HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>& other);

	/**
	 * Move assignment operator.
	 *
	 * @param other The hash table to move.
	 *
	 * @return Reference to this, after the move.
	 */
	HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>& operator=(HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>&& other) noexcept;

public:
	/** @return The load factor of the table. */
	FORCEINLINE float64 GetLoadFactor() const;

public:
	/**
	 * Gets the value associated with the given key.
	 * 
	 * @param The key to find.
	 * 
	 * @return Reference to the value associated to the given key.
	 */
	ValueType& At(const KeyType& key);

	/**
	 * Gets the value associated with the given key.
	 * Const version of the above.
	 * 
	 * @param The key to find.
	 * 
	 * @return Reference to the value associated to the given key.
	 */
	const ValueType& At(const KeyType& key) const;

	/**
	 * Gets the value stored at the given index in the table's internal array.
	 * 
	 * @param index The index where the value is stored.
	 * 
	 * @return Reference to the value stored at the given index.
	 */
	ValueType& AtIndex(usize index);

	/**
	 * Gets the value stored at the given index in the table's internal array.
	 * Const version of the above.
	 * 
	 * @param index The index where the value is stored.
	 * 
	 * @return Reference to the value stored at the given index.
	 */
	const ValueType& AtIndex(usize index) const;

	/**
	 * Index operator. If the key exists, its associated value is returned. Otherwise,
	 *   the key-value pair is inserted in the table.
	 * 
	 * @param key The key to find/insert.
	 * 
	 * @return Reference to the value (already present or newly inserted) associated
	 *   with the given key.
	 */
	ValueType& operator[](const KeyType& key);

	/**
	 * Index operator. If the key exists, its associated value is returned. Otherwise,
	 *   the key-value pair is inserted in the table.
	 * Move semantics version of the above.
	 * 
	 * @param key The key to find/insert.
	 * 
	 * @return Reference to the value (already present or newly inserted) associated
	 *   with the given key.
	 */
	ValueType& operator[](KeyType&& key);

	/**
	 * Index operator. The key must exist. It gets the value associated with the
	 *   given key. If the key doesn't exist, an assert will be triggered.
	 * 
	 * @param key The key to find.
	 * 
	 * @return Reference to the value associated with the given key.
	 */
	const ValueType& operator[](const KeyType& key) const;

public:
	/**
	 * Finds the index where the given key is stored, if it exists.
	 * 
	 * @param key The key to search for.
	 * 
	 * @return The index where the given key is stored, if it exists. If it doesn't exist,
	 *   it returns 'EndOfTable'.
	 */
	usize Find(const KeyType& key) const;

	/**
	 * Finds the index where the given key is stored.
	 * It assumes that the key must exist. If it doesn't, the application will crash.
	 *
	 * @param key The key to search for.
	 *
	 * @return The index where the given key is stored.
	 */
	usize FindExistingIndex(const KeyType& key) const;

	/**
	 * Finds the value associated with the given key.
	 * It assumes that the key must exist. If it doesn't, the application will crash.
	 * 
	 * @param key The key to search for.
	 * 
	 * @return Reference to the key associated with the given key.
	 */
	ValueType& FindExisting(const KeyType& key);

	/**
	 * Finds the value associated with the given key.
	 * It assumes that the key must exist. If it doesn't, the application will crash.
	 * Const version of the above.
	 * 
	 * @param key The key to search for.
	 * 
	 * @return Reference to the key associated with the given key.
	 */
	const ValueType& FindExisting(const KeyType& key) const;

public:
	/**
	 * Inserts an element in the container. If the key already exists,
	 *   there's no defined behavior. Usually though, an assertion will
	 *   be hit (if they are enabled).
	 * 
	 * @param key The key where to insert the element.
	 * 
	 * @param value The value to be inserted.
	 * 
	 * @return Reference to the inserted value (stored in the table's internal array).
	 */
	ValueType& Insert(const KeyType& key, const ValueType& value);

	/**
	 * Inserts an element in the container. If the key already exists,
	 *   there's no defined behavior. Usually though, an assertion will
	 *   be hit (if they are enabled).
	 * Move semantics version of the above.
	 * 
	 * @param key The key where to insert the element.
	 * 
	 * @param value The value to be inserted.
	 * 
	 * @return Reference to the inserted value (stored in the table's internal array).
	 */
	ValueType& Insert(const KeyType& key, ValueType&& value);

	/**
	 * Inserts an element in the container. If the key already exists,
	 *   there's no defined behavior. Usually though, an assertion will
	 *   be hit (if they are enabled).
	 * Move semantics version of the above.
	 * 
	 * @param key The key where to insert the element.
	 * 
	 * @param value The value to be inserted.
	 * 
	 * @return Reference to the inserted value (stored in the table's internal array).
	 */
	ValueType& Insert(KeyType&& key, const ValueType& value);

	/**
	 * Inserts an element in the container. If the key already exists,
	 *   there's no defined behavior. Usually though, an assertion will
	 *   be hit (if they are enabled).
	 * Move semantics version of the above.
	 * 
	 * @param key The key where to insert the element.
	 * 
	 * @param value The value to be inserted.
	 * 
	 * @return Reference to the inserted value (stored in the table's internal array).
	 */
	ValueType& Insert(KeyType&& key, ValueType&& value);

	/**
	 * Removes a key-value pair from the table.
	 * The key must exists in the table.
	 * 
	 * @param key The key to remove.
	 */
	void Remove(const KeyType& key);

	/**
	 * Removes the key-value pair from the table, stored at the given index.
	 * A valid key-value pair must be stored at the specified index.
	 * 
	 * @param key The key to remove.
	 */
	void RemoveIndex(usize index);

public:
	/**
	 * Clears the hash table. It doesn't shrink the table's internal memory block.
	 */
	void Clear();

public:
	/**
	 * Iterators over all the valid elements stored in the table.
	 * 
	 * @param func A function pointer, of type bool(const KeyType&, ValueType&). If this
	 *   function returns false, the iterating process will stop.
	 */
	template<typename Func>
	FORCEINLINE void ForEach(Func func);

	/**
	 * Iterators over all the valid elements stored in the table.
	 * Const version of the above.
	 * 
	 * @param func A function pointer, of type bool(const KeyType&, const ValueType&). If this
	 *   function returns false, the iterating process will stop.
	 */
	template<typename Func>
	FORCEINLINE void ForEach(Func func) const;

private:
	// Whether or not the table's load factor is greater than the maximum allowed load factor.
	FORCEINLINE bool IsOverLoadFactor() const;

	// Computes the table's natural next capacity.
	FORCEINLINE usize NextCapacity() const;

	// Computes the table's next capacity, taking into the account the number of elements it must contain.
	FORCEINLINE usize NextCapacity(usize additionalRequiredSize) const;

	// Computes the capacity for which the table can store the required number of elements.
	FORCEINLINE usize RequiredCapacityFor(usize requiredSize) const;

	// It reallocates the table's internal memory buffer/array.
	void ReAllocate(usize newCapacity);

	// It reallocates the table's internal memory buffer/array, without copying the elements over.
	void ReAllocateNoCopy(usize newCapacity);

	// Finds the first unoccupied index, starting with the given starting point/index.
	usize FindFirstUnoccupiedIndex(usize index) const;

	// Finds the index where the given key is stored, or the first unoccupied index if it doesn't exist.
	usize FindIndexOrFirstUnoccupied(const KeyType& key) const;

	// Inserts an element in the table.
	usize InternalInsert(const KeyType& key, const ValueType& value);

	// Inserts an element in the table.
	usize InternalInsert(const KeyType& key, ValueType&& value);

	// Inserts an element in the table.
	usize InternalInsert(KeyType&& key, const ValueType& value);

	// Inserts an element in the table.
	usize InternalInsert(KeyType&& key, ValueType&& value);

private:
	// Pointer to the memory block that stores the key-value pairs.
	KeyValue* m_KeyValues;

	// Pointer to the memory block that stores the bucket states.
	BucketState* m_States;

	// The capacity (in elements) of the key-value memory block.
	usize m_Capacity;

	// The number of valid elements currently stored in the container.
	usize m_Size;

	// The allocator instance used to performs all memory allocations/deallocations.
	AllocatorType m_AllocatorInstance;
};

//////////////// HASH-TABLE IMPLEMENTATION ////////////////

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::HashTable()
	: m_KeyValues(nullptr)
	, m_States(nullptr)
	, m_Capacity(0)
	, m_Size(0)
	, m_AllocatorInstance()
{}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::HashTable(const HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>& other)
	: m_KeyValues(nullptr)
	, m_States(nullptr)
	, m_Capacity(0)
	, m_Size(0)
	, m_AllocatorInstance()
{
	ReAllocateNoCopy(RequiredCapacityFor(other.m_Size));

	for (usize i = 0; i < other.m_Capacity; ++i)
	{
		if (other.m_States[i] == BucketState::Occupied)
		{
			const KeyValue& keyValue = other.m_KeyValues[i];
			InternalInsert(keyValue.Key, keyValue.Value);
		}
	}
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::HashTable(HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>&& other) noexcept
	: m_KeyValues(nullptr)
	, m_States(nullptr)
	, m_Capacity(0)
	, m_Size(0)
	, m_AllocatorInstance()
{
	if (m_AllocatorInstance == other.m_AllocatorInstance)
	{
		m_KeyValues = other.m_KeyValues;
		m_States = other.m_States;
		m_Capacity = other.m_Capacity;
		m_Size = other.m_Size;

		other.m_KeyValues = nullptr;
		other.m_States = nullptr;
		other.m_Capacity = 0;
		other.m_Size = 0;
	}
	else
	{
		ReAllocateNoCopy(RequiredCapacityFor(other.m_Size));

		for (usize i = 0; i < other.m_Capacity; ++i)
		{
			if (other.m_States[i] == BucketState::Occupied)
			{
				KeyValue& keyValue = other.m_KeyValues[i];
				InternalInsert(Types::Move(keyValue.Key), Types::Move(keyValue.Value));
				keyValue.Key.~KeyType();
				keyValue.Value.~ValueType();
			}

			other.m_Size = 0;
			Memory::Set(other.m_States, (uint8)BucketState::Empty, other.m_Capacity * sizeof(BucketState));
		}
	}
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::~HashTable()
{
	Clear();
	m_AllocatorInstance.Free(m_KeyValues, m_Capacity * (sizeof(KeyValue) + sizeof(BucketState)));
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::operator=(const HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>& other)
{
	Clear();

	const usize requiredCapacity = RequiredCapacityFor(other.m_Size);
	if (requiredCapacity > m_Capacity)
	{
		ReAllocateNoCopy(requiredCapacity);
	}

	for (usize i = 0; i < other.m_Capacity; ++i)
	{
		if (other.m_States[i] == BucketState::Occupied)
		{
			const KeyValue& keyValue = other.m_KeyValues[i];
			InternalInsert(keyValue.Key, keyValue.Value);
		}
	}

	return *this;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::operator=(HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>&& other) noexcept
{
	Clear();

	if (m_AllocatorInstance == other.m_AllocatorInstance)
	{
		KeyValue* tempKeyValues = m_KeyValues;
		BucketState* tempStates = m_States;
		usize tempCapacity = m_Capacity;

		m_KeyValues = other.m_KeyValues;
		m_States = other.m_States;
		m_Capacity = other.m_Capacity;
		m_Size = other.m_Size;

		other.m_KeyValues = tempKeyValues;
		other.m_States = tempStates;
		other.m_Capacity = tempCapacity;
		other.m_Size = 0;
	}
	else
	{
		const usize requiredCapacity = RequiredCapacityFor(other.m_Size);
		if (requiredCapacity > m_Capacity)
		{
			ReAllocateNoCopy(requiredCapacity);
		}

		for (usize i = 0; i < other.m_Capacity; ++i)
		{
			if (other.m_States[i] == BucketState::Occupied)
			{
				KeyValue& keyValue = other.m_KeyValues[i];
				InternalInsert(Types::Move(keyValue.Key), Types::Move(keyValue.Value));
				keyValue.Key.~KeyType();
				keyValue.Value.~ValueType();
			}
		}

		other.m_Size = 0;
		Memory::Set(other.m_States, (uint8)BucketState::Empty, other.m_Capacity * sizeof(BucketState));
	}

	return *this;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
FORCEINLINE float64 HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::GetLoadFactor() const
{
	return (float64)m_Size / (float64)m_Capacity;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::At(const KeyType& key)
{
	return FindExisting(key);
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
const ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::At(const KeyType& key) const
{
	return FindExisting(key);
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::AtIndex(usize index)
{
	HC_DASSERT(m_States[index] == BucketState::Occupied); // No element is stored at the given index!
	return m_KeyValues[index].Value;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
const ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::AtIndex(usize index) const
{
	HC_DASSERT(m_States[index] == BucketState::Occupied); // No element is stored at the given index!
	return m_KeyValues[index].Value;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::operator[](const KeyType& key)
{
	const usize index = FindIndexOrFirstUnoccupied(key);

	if (m_States[index] != BucketState::Occupied)
	{
		new (&m_KeyValues[index].Key)   KeyType(key);
		new (&m_KeyValues[index].Value) ValueType();
		m_States[index] = BucketState::Occupied;
		m_Size++;
	}

	return m_KeyValues[index].Value;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::operator[](KeyType&& key)
{
	const usize index = FindIndexOrFirstUnoccupied(key);

	if (m_States[index] != BucketState::Occupied)
	{
		new (&m_KeyValues[index].Key)   KeyType(Types::Move(key));
		new (&m_KeyValues[index].Value) ValueType();
		m_States[index] = BucketState::Occupied;
		m_Size++;
	}

	return m_KeyValues[index].Value;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
const ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::operator[](const KeyType& key) const
{
	return At(key);
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
usize HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::Find(const KeyType& key) const
{
	if (m_Capacity == 0)
	{
		return EndOfTable;
	}
	usize index = Hasher::Compute<KeyType>(key) % m_Capacity;

	for (usize i = 0; i < m_Capacity; ++i)
	{
		if (m_States[index] == BucketState::Occupied && Comparator::Compare<KeyType>(key, m_KeyValues[index].Key))
		{
			return index;
		}
		if (m_States[index] == BucketState::Empty)
		{
			break;
		}

		index = (index + 1) % m_Capacity;
	}

	return EndOfTable;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
usize HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::FindExistingIndex(const KeyType& key) const
{
	usize index = Hasher::Compute<KeyType>(key) % m_Capacity;

	while (true)
	{
		if (m_States[index] == BucketState::Occupied && Comparator::Compare<KeyType>(key, m_KeyValues[index].Key))
		{
			return index;
		}

		index = (index + 1) % m_Capacity;
	}
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::FindExisting(const KeyType& key)
{
	return m_KeyValues[FindExistingIndex(key)].Value;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
const ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::FindExisting(const KeyType& key) const
{
	return m_KeyValues[FindExistingIndex(key)].Value;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::Insert(const KeyType& key, const ValueType& value)
{
	if (IsOverLoadFactor())
	{
		ReAllocate(NextCapacity());
	}

#if HC_ENABLE_ASSERTS

	const usize index = FindIndexOrFirstUnoccupied(key);
	HC_ASSERT(m_States[index] != BucketState::Occupied); // Key already exists in the table!

	new (&m_KeyValues[index].Key)   KeyType  (key);
	new (&m_KeyValues[index].Value) ValueType(value);
	m_States[index] = BucketState::Occupied;
	m_Size++;

#else

	const usize index = InternalInsert(key, value);

#endif // HC_ENABLE_ASSERTS

	return m_KeyValues[index].Value;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::Insert(const KeyType& key, ValueType&& value)
{
	if (IsOverLoadFactor())
	{
		ReAllocate(NextCapacity());
	}

#if HC_ENABLE_ASSERTS

	const usize index = FindIndexOrFirstUnoccupied(key);
	HC_ASSERT(m_States[index] != BucketState::Occupied); // Key already exists in the table!

	new (&m_KeyValues[index].Key)   KeyType(key);
	new (&m_KeyValues[index].Value) ValueType(Types::Move(value));
	m_States[index] = BucketState::Occupied;
	m_Size++;

#else

	const usize index = InternalInsert(key, Types::Move(value));

#endif // HC_ENABLE_ASSERTS

	return m_KeyValues[index].Value;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::Insert(KeyType&& key, const ValueType& value)
{
	if (IsOverLoadFactor())
	{
		ReAllocate(NextCapacity());
	}

#if HC_ENABLE_ASSERTS

	const usize index = FindIndexOrFirstUnoccupied(key);
	HC_ASSERT(m_States[index] != BucketState::Occupied); // Key already exists in the table!

	new (&m_KeyValues[index].Key)   KeyType(Types::Move(key));
	new (&m_KeyValues[index].Value) ValueType(value);
	m_States[index] = BucketState::Occupied;
	m_Size++;

#else

	const usize index = InternalInsert(Types::Move(key), value);

#endif // HC_ENABLE_ASSERTS

	return m_KeyValues[index].Value;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::Insert(KeyType&& key, ValueType&& value)
{
	if (IsOverLoadFactor())
	{
		ReAllocate(NextCapacity());
	}

#if HC_ENABLE_ASSERTS

	const usize index = FindIndexOrFirstUnoccupied(key);
	HC_ASSERT(m_States[index] != BucketState::Occupied); // Key already exists in the table!

	new (&m_KeyValues[index].Key)   KeyType(Types::Move(key));
	new (&m_KeyValues[index].Value) ValueType(Types::Move(value));
	m_States[index] = BucketState::Occupied;
	m_Size++;

#else

	const usize index = InternalInsert(Types::Move(key), Types::Move(value));

#endif // HC_ENABLE_ASSERTS

	return m_KeyValues[index].Value;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
void HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::Remove(const KeyType& key)
{
	const usize index = FindExistingIndex(key);

	KeyValue& keyValue = m_KeyValues[index];
	keyValue.Key.~KeyType();
	keyValue.Value.~ValueType();
	m_States[index] = BucketState::Deleted;
	m_Size--;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
void HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::RemoveIndex(usize index)
{
	HC_ASSERT(m_States[index] == BucketState::Occupied); // No element is stored at the given index!
	
	KeyValue& keyValue = m_KeyValues[index];
	keyValue.Key.~KeyType();
	keyValue.Value.~ValueType();
	m_States[index] = BucketState::Deleted;
	m_Size--;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
void HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::Clear()
{
	if (m_Size == 0)
	{
		Memory::Set(m_States, (uint8)BucketState::Empty, m_Capacity * sizeof(BucketState));
		return;
	}

	for (usize i = 0; i < m_Capacity; ++i)
	{
		if (m_States[i] == BucketState::Occupied)
		{
			m_KeyValues[i].Key.~KeyType();
			m_KeyValues[i].Value.~ValueType();
		}
	}

	m_Size = 0;
	Memory::Set(m_States, (uint8)BucketState::Empty, m_Capacity * sizeof(BucketState));
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
template<typename Func>
FORCEINLINE void HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::ForEach(Func func)
{
	if (m_Size == 0)
	{
		return;
	}

	for (usize i = 0; i < m_Capacity; ++i)
	{
		if (m_States[i] == BucketState::Occupied)
		{
			KeyValue& keyValue = m_KeyValues[i];
			if (!func(keyValue.Key, keyValue.Value))
			{
				break;
			}
		}
	}
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
template<typename Func>
FORCEINLINE void HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::ForEach(Func func) const
{
	if (m_Size == 0)
	{
		return;
	}

	for (usize i = 0; i < m_Capacity; ++i)
	{
		if (m_States[i] == BucketState::Occupied)
		{
			const KeyValue& keyValue = m_KeyValues[i];
			if (!func(keyValue.Key, keyValue.Value))
			{
				break;
			}
		}
	}
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
FORCEINLINE bool HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::IsOverLoadFactor() const
{
	return (m_Capacity == 0) || (GetLoadFactor() >= MaxLoadFactor);
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
FORCEINLINE usize HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::NextCapacity() const
{
	return m_Capacity * 2 + 2;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
FORCEINLINE usize HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::NextCapacity(usize additionalRequiredSize) const
{
	const usize nextCapacity = NextCapacity();
	const usize requiredCapacity = (usize)((float64)(m_Size + additionalRequiredSize) / MaxLoadFactor) + 1;
	return nextCapacity > requiredCapacity ? nextCapacity : requiredCapacity;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
FORCEINLINE usize HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::RequiredCapacityFor(usize requiredSize) const
{
	return (usize)((float64)requiredSize / MaxLoadFactor) + 1;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
void HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::ReAllocate(usize newCapacity)
{
	void* newBlock = m_AllocatorInstance.AllocateTaggedI(newCapacity * (sizeof(KeyValue) + sizeof(BucketState)));

	KeyValue* newKeyValues = (KeyValue*)newBlock;
	BucketState* newStates = (BucketState*)(newKeyValues + newCapacity);

	Memory::Set(newStates, (uint8)BucketState::Empty, newCapacity * sizeof(BucketState));

	KeyValue* oldKeyValues = m_KeyValues;
	BucketState* oldStates = m_States;
	usize oldCapacity = m_Capacity;

	m_KeyValues = newKeyValues;
	m_States = newStates;
	m_Capacity = newCapacity;
	m_Size = 0;

	for (usize i = 0; i < oldCapacity; ++i)
	{
		if (oldStates[i] == BucketState::Occupied)
		{
			InternalInsert(Types::Move(oldKeyValues[i].Key), Types::Move(oldKeyValues[i].Value));
		}
	}

	m_AllocatorInstance.Free(oldKeyValues, oldCapacity * (sizeof(KeyValue) + sizeof(BucketState)));
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
void HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::ReAllocateNoCopy(usize newCapacity)
{
	if (m_Size > 0)
	{
		for (usize i = 0; i < m_Capacity; ++i)
		{
			if (m_States[i] == BucketState::Occupied)
			{
				m_KeyValues[i].Key.~KeyType();
				m_KeyValues[i].Value.~ValueType();
			}
		}
	}
	m_AllocatorInstance.Free(m_KeyValues, m_Capacity * (sizeof(KeyValue) + sizeof(BucketState)));

	void* newBlock = m_AllocatorInstance.AllocateTaggedI(newCapacity * (sizeof(KeyValue) + sizeof(BucketState)));

	KeyValue* newKeyValues = (KeyValue*)newBlock;
	BucketState* newStates = (BucketState*)(newKeyValues + newCapacity);

	Memory::Set(newStates, BucketState::Empty, newCapacity * sizeof(BucketState));

	m_KeyValues = newKeyValues;
	m_States = newStates;
	m_Capacity = newCapacity;
	m_Size = 0;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
usize HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::FindFirstUnoccupiedIndex(usize index) const
{
	while (true)
	{
		if (m_States[index] != BucketState::Occupied)
		{
			return index;
		}

		index = (index + 1) % m_Capacity;
	}
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
usize HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::FindIndexOrFirstUnoccupied(const KeyType& key) const
{
	usize index = Hasher::Compute<KeyType>(key) % m_Capacity;
	usize firstIndex = static_cast<usize>(-1);

	for (usize i = 0; i < m_Capacity; ++i)
	{
		if (m_States[index] == BucketState::Occupied)
		{
			if (Comparator::Compare<KeyType>(key, m_KeyValues[index].Key))
			{
				return index;
			}
		}
		else if (m_States[index] == BucketState::Deleted)
		{
			if (firstIndex == static_cast<usize>(-1))
			{
				firstIndex = index;
			}
		}
		else if (m_States[index] == BucketState::Empty)
		{
			if (firstIndex == static_cast<usize>(-1))
			{
				firstIndex = index;
			}
			break;
		}

		index = (index + 1) % m_Capacity;
	}

	return firstIndex;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
usize HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::InternalInsert(const KeyType& key, const ValueType& value)
{
	const usize index = FindFirstUnoccupiedIndex(Hasher::Compute<KeyType>(key) % m_Capacity);

	new (&m_KeyValues[index].Key)   KeyType  (key);
	new (&m_KeyValues[index].Value) ValueType(value);
	m_States[index] = BucketState::Occupied;

	m_Size++;
	return index;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
usize HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::InternalInsert(const KeyType& key, ValueType&& value)
{
	const usize index = FindFirstUnoccupiedIndex(Hasher::Compute<KeyType>(key) % m_Capacity);

	new (&m_KeyValues[index].Key)   KeyType  (key);
	new (&m_KeyValues[index].Value) ValueType(Types::Move(value));
	m_States[index] = BucketState::Occupied;

	m_Size++;
	return index;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
usize HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::InternalInsert(KeyType&& key, const ValueType& value)
{
	const usize index = FindFirstUnoccupiedIndex(Hasher::Compute<KeyType>(key) % m_Capacity);

	new (&m_KeyValues[index].Key)   KeyType  (Types::Move(key));
	new (&m_KeyValues[index].Value) ValueType(value);
	m_States[index] = BucketState::Occupied;

	m_Size++;
	return index;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
usize HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::InternalInsert(KeyType&& key, ValueType&& value)
{
	const usize index = FindFirstUnoccupiedIndex(Hasher::Compute<KeyType>(key) % m_Capacity);

	new (&m_KeyValues[index].Key)   KeyType  (Types::Move(key));
	new (&m_KeyValues[index].Value) ValueType(Types::Move(value));
	m_States[index] = BucketState::Occupied;

	m_Size++;
	return index;
}

} // namespace HC