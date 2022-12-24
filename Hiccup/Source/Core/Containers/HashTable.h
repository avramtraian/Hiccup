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
		KeyType     key;
		ValueType   value;
	};

	// The state that a bucket can have.
	enum class BucketState : uint8_t
	{
		Empty = 0x00, Deleted = 0xDD, Occupied = 0xFF
	};

	// The maximum allowed table load factor.
	static constexpr float64_t MaxLoadFactor = 0.75;

	static constexpr size_t EndOfTable = static_cast<size_t>(-1);

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
	ALWAYS_INLINE float64_t get_load_factor() const;

public:
	/**
	 * Gets the value associated with the given key.
	 * 
	 * @param The key to find.
	 * 
	 * @return Reference to the value associated to the given key.
	 */
	ValueType& at(const KeyType& key);

	/**
	 * Gets the value associated with the given key.
	 * Const version of the above.
	 * 
	 * @param The key to find.
	 * 
	 * @return Reference to the value associated to the given key.
	 */
	const ValueType& at(const KeyType& key) const;

	/**
	 * Gets the value stored at the given index in the table's internal array.
	 * 
	 * @param index The index where the value is stored.
	 * 
	 * @return Reference to the value stored at the given index.
	 */
	ValueType& at_index(size_t index);

	/**
	 * Gets the value stored at the given index in the table's internal array.
	 * Const version of the above.
	 * 
	 * @param index The index where the value is stored.
	 * 
	 * @return Reference to the value stored at the given index.
	 */
	const ValueType& at_index(size_t index) const;

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
	size_t find(const KeyType& key) const;

	/**
	 * Finds the index where the given key is stored.
	 * It assumes that the key must exist. If it doesn't, the application will crash.
	 *
	 * @param key The key to search for.
	 *
	 * @return The index where the given key is stored.
	 */
	size_t find_existing_index(const KeyType& key) const;

	/**
	 * Finds the value associated with the given key.
	 * It assumes that the key must exist. If it doesn't, the application will crash.
	 * 
	 * @param key The key to search for.
	 * 
	 * @return Reference to the key associated with the given key.
	 */
	ValueType& find_existing(const KeyType& key);

	/**
	 * Finds the value associated with the given key.
	 * It assumes that the key must exist. If it doesn't, the application will crash.
	 * Const version of the above.
	 * 
	 * @param key The key to search for.
	 * 
	 * @return Reference to the key associated with the given key.
	 */
	const ValueType& find_existing(const KeyType& key) const;

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
	ValueType& insert(const KeyType& key, const ValueType& value);

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
	ValueType& insert(const KeyType& key, ValueType&& value);

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
	ValueType& insert(KeyType&& key, const ValueType& value);

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
	ValueType& insert(KeyType&& key, ValueType&& value);

	/**
	 * Removes a key-value pair from the table.
	 * The key must exists in the table.
	 * 
	 * @param key The key to remove.
	 */
	void remove(const KeyType& key);

	/**
	 * Removes the key-value pair from the table, stored at the given index.
	 * A valid key-value pair must be stored at the specified index.
	 * 
	 * @param key The key to remove.
	 */
	void remove_index(size_t index);

public:
	/**
	 * Clears the hash table. It doesn't shrink the table's internal memory block.
	 */
	void clear();

public:
	/**
	 * Iterators over all the valid elements stored in the table.
	 * 
	 * @param func A function pointer, of type bool(const KeyType&, ValueType&). If this
	 *   function returns false, the iterating process will stop.
	 */
	template<typename Func>
	ALWAYS_INLINE void for_each(Func func);

	/**
	 * Iterators over all the valid elements stored in the table.
	 * Const version of the above.
	 * 
	 * @param func A function pointer, of type bool(const KeyType&, const ValueType&). If this
	 *   function returns false, the iterating process will stop.
	 */
	template<typename Func>
	ALWAYS_INLINE void for_each(Func func) const;

private:
	// Whether or not the table's load factor is greater than the maximum allowed load factor.
	ALWAYS_INLINE bool is_over_load_factor() const;

	// Computes the table's natural next capacity.
	ALWAYS_INLINE size_t calculate_growth() const;

	// Computes the table's next capacity, taking into the account the number of elements it must contain.
	ALWAYS_INLINE size_t calculate_growth(size_t additional_required_size) const;

	// Computes the capacity for which the table can store the required number of elements.
	ALWAYS_INLINE size_t required_capacity_for(size_t required_size) const;

	// It reallocates the table's internal memory buffer/array.
	void re_allocate(size_t new_capacity);

	// It reallocates the table's internal memory buffer/array, without copying the elements over.
	void re_allocate_no_copy(size_t new_capacity);

	// Finds the first unoccupied index, starting with the given starting point/index.
	size_t find_first_unoccupied_index(size_t index) const;

	// Finds the index where the given key is stored, or the first unoccupied index if it doesn't exist.
	size_t find_index_of_first_unoccupied(const KeyType& key) const;

	// Inserts an element in the table.
	size_t internal_insert(const KeyType& key, const ValueType& value);

	// Inserts an element in the table.
	size_t internal_insert(const KeyType& key, ValueType&& value);

	// Inserts an element in the table.
	size_t internal_insert(KeyType&& key, const ValueType& value);

	// Inserts an element in the table.
	size_t internal_insert(KeyType&& key, ValueType&& value);

private:
	// Pointer to the memory block that stores the key-value pairs.
	KeyValue* m_key_values;

	// Pointer to the memory block that stores the bucket states.
	BucketState* m_states;

	// The capacity (in elements) of the key-value memory block.
	size_t m_capacity;

	// The number of valid elements currently stored in the container.
	size_t m_size;

	// The allocator instance used to performs all memory allocations/deallocations.
	AllocatorType m_allocator_instance;
};

//////////////// HASH-TABLE IMPLEMENTATION ////////////////

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::HashTable()
	: m_key_values(nullptr)
	, m_states(nullptr)
	, m_capacity(0)
	, m_size(0)
	, m_allocator_instance()
{}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::HashTable(const HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>& other)
	: m_key_values(nullptr)
	, m_states(nullptr)
	, m_capacity(0)
	, m_size(0)
	, m_allocator_instance()
{
	re_allocate_no_copy(required_capacity_for(other.m_size));

	for (size_t i = 0; i < other.m_capacity; ++i)
	{
		if (other.m_states[i] == BucketState::Occupied)
		{
			const KeyValue& key_value = other.m_key_values[i];
			internal_insert(key_value.key, key_value.value);
		}
	}
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::HashTable(HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>&& other) noexcept
	: m_key_values(nullptr)
	, m_states(nullptr)
	, m_capacity(0)
	, m_size(0)
	, m_allocator_instance()
{
	if (m_allocator_instance == other.m_allocator_instance)
	{
		m_key_values = other.m_key_values;
		m_states = other.m_states;
		m_capacity = other.m_capacity;
		m_size = other.m_size;

		other.m_key_values = nullptr;
		other.m_states = nullptr;
		other.m_capacity = 0;
		other.m_size = 0;
	}
	else
	{
		re_allocate_no_copy(required_capacity_for(other.m_size));

		for (size_t i = 0; i < other.m_capacity; ++i)
		{
			if (other.m_states[i] == BucketState::Occupied)
			{
				KeyValue& key_value = other.m_key_values[i];
				internal_insert(Types::move(key_value.key), Types::move(key_value.value));
				key_value.key.~KeyType();
				key_value.value.~ValueType();
			}

			other.m_size = 0;
			Memory::set(other.m_states, (uint8_t)BucketState::Empty, other.m_capacity * sizeof(BucketState));
		}
	}
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::~HashTable()
{
	clear();
	m_allocator_instance.free(m_key_values, m_capacity * (sizeof(KeyValue) + sizeof(BucketState)));
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::operator=(const HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>& other)
{
	clear();

	const size_t requiredCapacity = required_capacity_for(other.m_size);
	if (requiredCapacity > m_capacity)
	{
		re_allocate_no_copy(requiredCapacity);
	}

	for (size_t i = 0; i < other.m_capacity; ++i)
	{
		if (other.m_states[i] == BucketState::Occupied)
		{
			const KeyValue& key_value = other.m_key_values[i];
			internal_insert(key_value.key, key_value.value);
		}
	}

	return *this;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::operator=(HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>&& other) noexcept
{
	clear();

	if (m_allocator_instance == other.m_allocator_instance)
	{
		KeyValue* tempKeyValues = m_key_values;
		BucketState* tempStates = m_states;
		size_t tempCapacity = m_capacity;

		m_key_values = other.m_key_values;
		m_states = other.m_states;
		m_capacity = other.m_capacity;
		m_size = other.m_size;

		other.m_key_values = tempKeyValues;
		other.m_states = tempStates;
		other.m_capacity = tempCapacity;
		other.m_size = 0;
	}
	else
	{
		const size_t requiredCapacity = required_capacity_for(other.m_size);
		if (requiredCapacity > m_capacity)
		{
			re_allocate_no_copy(requiredCapacity);
		}

		for (size_t i = 0; i < other.m_capacity; ++i)
		{
			if (other.m_states[i] == BucketState::Occupied)
			{
				KeyValue& key_value = other.m_key_values[i];
				internal_insert(Types::move(key_value.key), Types::move(key_value.value));
				key_value.key.~KeyType();
				key_value.value.~ValueType();
			}
		}

		other.m_size = 0;
		Memory::set(other.m_states, (uint8_t)BucketState::Empty, other.m_capacity * sizeof(BucketState));
	}

	return *this;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
ALWAYS_INLINE float64_t HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::get_load_factor() const
{
	return (float64_t)m_size / (float64_t)m_capacity;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::at(const KeyType& key)
{
	return find_existing(key);
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
const ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::at(const KeyType& key) const
{
	return find_existing(key);
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::at_index(size_t index)
{
	HC_DASSERT(m_states[index] == BucketState::Occupied); // No element is stored at the given index!
	return m_key_values[index].value;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
const ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::at_index(size_t index) const
{
	HC_DASSERT(m_states[index] == BucketState::Occupied); // No element is stored at the given index!
	return m_key_values[index].value;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::operator[](const KeyType& key)
{
	const size_t index = find_index_of_first_unoccupied(key);

	if (m_states[index] != BucketState::Occupied)
	{
		new (&m_key_values[index].key)   KeyType(key);
		new (&m_key_values[index].value) ValueType();
		m_states[index] = BucketState::Occupied;
		m_size++;
	}

	return m_key_values[index].value;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::operator[](KeyType&& key)
{
	const size_t index = find_index_of_first_unoccupied(key);

	if (m_states[index] != BucketState::Occupied)
	{
		new (&m_key_values[index].key)   KeyType(Types::move(key));
		new (&m_key_values[index].value) ValueType();
		m_states[index] = BucketState::Occupied;
		m_size++;
	}

	return m_key_values[index].value;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
const ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::operator[](const KeyType& key) const
{
	return At(key);
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
size_t HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::find(const KeyType& key) const
{
	if (m_capacity == 0)
	{
		return EndOfTable;
	}
	size_t index = Hasher::compute<KeyType>(key) % m_capacity;

	for (size_t i = 0; i < m_capacity; ++i)
	{
		if (m_states[index] == BucketState::Occupied && Comparator::compare<KeyType>(key, m_key_values[index].key))
		{
			return index;
		}
		if (m_states[index] == BucketState::Empty)
		{
			break;
		}

		index = (index + 1) % m_capacity;
	}

	return EndOfTable;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
size_t HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::find_existing_index(const KeyType& key) const
{
	size_t index = Hasher::compute<KeyType>(key) % m_capacity;

	while (true)
	{
		if (m_states[index] == BucketState::Occupied && Comparator::compare<KeyType>(key, m_key_values[index].key))
		{
			return index;
		}

		index = (index + 1) % m_capacity;
	}
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::find_existing(const KeyType& key)
{
	return m_key_values[FindExistingIndex(key)].value;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
const ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::find_existing(const KeyType& key) const
{
	return m_key_values[FindExistingIndex(key)].value;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::insert(const KeyType& key, const ValueType& value)
{
	if (is_over_load_factor())
	{
		re_allocate(calculate_growth());
	}

#if HC_ENABLE_ASSERTS

	const size_t index = find_index_of_first_unoccupied(key);
	HC_ASSERT(m_states[index] != BucketState::Occupied); // Key already exists in the table!

	new (&m_key_values[index].key)   KeyType  (key);
	new (&m_key_values[index].value) ValueType(value);
	m_states[index] = BucketState::Occupied;
	m_size++;

#else

	const size_t index = internal_insert(key, value);

#endif // HC_ENABLE_ASSERTS

	return m_key_values[index].value;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::insert(const KeyType& key, ValueType&& value)
{
	if (is_over_load_factor())
	{
		re_allocate(calculate_growth());
	}

#if HC_ENABLE_ASSERTS

	const size_t index = find_index_of_first_unoccupied(key);
	HC_ASSERT(m_states[index] != BucketState::Occupied); // Key already exists in the table!

	new (&m_key_values[index].key)   KeyType(key);
	new (&m_key_values[index].value) ValueType(Types::move(value));
	m_states[index] = BucketState::Occupied;
	m_size++;

#else

	const size_t index = internal_insert(key, Types::move(value));

#endif // HC_ENABLE_ASSERTS

	return m_key_values[index].value;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::insert(KeyType&& key, const ValueType& value)
{
	if (is_over_load_factor())
	{
		re_allocate(calculate_growth());
	}

#if HC_ENABLE_ASSERTS

	const size_t index = find_index_of_first_unoccupied(key);
	HC_ASSERT(m_states[index] != BucketState::Occupied); // Key already exists in the table!

	new (&m_key_values[index].key)   KeyType(Types::move(key));
	new (&m_key_values[index].value) ValueType(value);
	m_states[index] = BucketState::Occupied;
	m_size++;

#else

	const size_t index = internal_insert(Types::move(key), value);

#endif // HC_ENABLE_ASSERTS

	return m_key_values[index].value;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
ValueType& HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::insert(KeyType&& key, ValueType&& value)
{
	if (is_over_load_factor())
	{
		re_allocate(calculate_growth());
	}

#if HC_ENABLE_ASSERTS

	const size_t index = find_index_of_first_unoccupied(key);
	HC_ASSERT(m_states[index] != BucketState::Occupied); // Key already exists in the table!

	new (&m_key_values[index].key)   KeyType(Types::move(key));
	new (&m_key_values[index].value) ValueType(Types::move(value));
	m_states[index] = BucketState::Occupied;
	m_size++;

#else

	const size_t index = internal_insert(Types::move(key), Types::move(value));

#endif // HC_ENABLE_ASSERTS

	return m_key_values[index].value;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
void HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::remove(const KeyType& key)
{
	const size_t index = FindExistingIndex(key);

	KeyValue& key_value = m_key_values[index];
	key_value.key.~KeyType();
	key_value.value.~ValueType();
	m_states[index] = BucketState::Deleted;
	m_size--;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
void HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::remove_index(size_t index)
{
	HC_ASSERT(m_states[index] == BucketState::Occupied); // No element is stored at the given index!
	
	KeyValue& key_value = m_key_values[index];
	key_value.key.~KeyType();
	key_value.value.~ValueType();
	m_states[index] = BucketState::Deleted;
	m_size--;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
void HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::clear()
{
	if (m_size == 0)
	{
		Memory::set(m_states, (uint8_t)BucketState::Empty, m_capacity * sizeof(BucketState));
		return;
	}

	for (size_t i = 0; i < m_capacity; ++i)
	{
		if (m_states[i] == BucketState::Occupied)
		{
			m_key_values[i].key.~KeyType();
			m_key_values[i].value.~ValueType();
		}
	}

	m_size = 0;
	Memory::set(m_states, (uint8_t)BucketState::Empty, m_capacity * sizeof(BucketState));
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
template<typename Func>
ALWAYS_INLINE void HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::for_each(Func func)
{
	if (m_size == 0)
	{
		return;
	}

	for (size_t i = 0; i < m_capacity; ++i)
	{
		if (m_states[i] == BucketState::Occupied)
		{
			KeyValue& key_value = m_key_values[i];
			if (!func(key_value.key, key_value.value))
			{
				break;
			}
		}
	}
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
template<typename Func>
ALWAYS_INLINE void HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::for_each(Func func) const
{
	if (m_size == 0)
	{
		return;
	}

	for (size_t i = 0; i < m_capacity; ++i)
	{
		if (m_states[i] == BucketState::Occupied)
		{
			const KeyValue& key_value = m_key_values[i];
			if (!func(key_value.key, key_value.value))
			{
				break;
			}
		}
	}
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
ALWAYS_INLINE bool HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::is_over_load_factor() const
{
	return (m_capacity == 0) || (get_load_factor() >= MaxLoadFactor);
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
ALWAYS_INLINE size_t HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::calculate_growth() const
{
	return m_capacity * 2 + 2;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
ALWAYS_INLINE size_t HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::calculate_growth(size_t additionalRequiredSize) const
{
	const size_t nextCapacity = calculate_growth();
	const size_t requiredCapacity = (size_t)((float64_t)(m_size + additionalRequiredSize) / MaxLoadFactor) + 1;
	return nextCapacity > requiredCapacity ? nextCapacity : requiredCapacity;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
ALWAYS_INLINE size_t HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::required_capacity_for(size_t requiredSize) const
{
	return (size_t)((float64_t)requiredSize / MaxLoadFactor) + 1;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
void HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::re_allocate(size_t newCapacity)
{
	void* new_data = m_allocator_instance.allocate_tagged_i(newCapacity * (sizeof(KeyValue) + sizeof(BucketState)));

	KeyValue* new_key_values = (KeyValue*)new_data;
	BucketState* new_states = (BucketState*)(new_key_values + newCapacity);

	Memory::set(new_states, (uint8_t)BucketState::Empty, newCapacity * sizeof(BucketState));

	KeyValue* old_key_values = m_key_values;
	BucketState* old_states = m_states;
	size_t old_capacity = m_capacity;

	m_key_values = new_key_values;
	m_states = new_states;
	m_capacity = newCapacity;
	m_size = 0;

	for (size_t i = 0; i < old_capacity; ++i)
	{
		if (old_states[i] == BucketState::Occupied)
		{
			internal_insert(Types::move(old_key_values[i].key), Types::move(old_key_values[i].value));
		}
	}

	m_allocator_instance.free(old_key_values, old_capacity * (sizeof(KeyValue) + sizeof(BucketState)));
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
void HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::re_allocate_no_copy(size_t newCapacity)
{
	if (m_size > 0)
	{
		for (size_t i = 0; i < m_capacity; ++i)
		{
			if (m_states[i] == BucketState::Occupied)
			{
				m_key_values[i].key.~KeyType();
				m_key_values[i].value.~ValueType();
			}
		}
	}
	m_allocator_instance.free(m_key_values, m_capacity * (sizeof(KeyValue) + sizeof(BucketState)));

	void* new_data = m_allocator_instance.allocate_tagged_i(newCapacity * (sizeof(KeyValue) + sizeof(BucketState)));


	m_key_values = (KeyValue*)new_data;
	m_states = (BucketState*)(new_key_values + newCapacity);
	m_capacity = newCapacity;
	m_size = 0;

	Memory::set(m_States, BucketState::Empty, m_Capacity * sizeof(BucketState));
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
size_t HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::find_first_unoccupied_index(size_t index) const
{
	while (true)
	{
		if (m_states[index] != BucketState::Occupied)
		{
			return index;
		}

		index = (index + 1) % m_capacity;
	}
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
size_t HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::find_index_of_first_unoccupied(const KeyType& key) const
{
	size_t index = Hasher::compute<KeyType>(key) % m_capacity;
	size_t firstIndex = static_cast<size_t>(-1);

	for (size_t i = 0; i < m_capacity; ++i)
	{
		if (m_states[index] == BucketState::Occupied)
		{
			if (Comparator::compare<KeyType>(key, m_key_values[index].key))
			{
				return index;
			}
		}
		else if (m_states[index] == BucketState::Deleted)
		{
			if (firstIndex == static_cast<size_t>(-1))
			{
				firstIndex = index;
			}
		}
		else if (m_states[index] == BucketState::Empty)
		{
			if (firstIndex == static_cast<size_t>(-1))
			{
				firstIndex = index;
			}
			break;
		}

		index = (index + 1) % m_capacity;
	}

	return firstIndex;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
size_t HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::internal_insert(const KeyType& key, const ValueType& value)
{
	const size_t index = find_first_unoccupied_index(Hasher::compute<KeyType>(key) % m_capacity);

	new (&m_key_values[index].key)   KeyType  (key);
	new (&m_key_values[index].value) ValueType(value);
	m_states[index] = BucketState::Occupied;

	m_size++;
	return index;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
size_t HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::internal_insert(const KeyType& key, ValueType&& value)
{
	const size_t index = find_first_unoccupied_index(Hasher::compute<KeyType>(key) % m_capacity);

	new (&m_key_values[index].key)   KeyType  (key);
	new (&m_key_values[index].value) ValueType(Types::move(value));
	m_states[index] = BucketState::Occupied;

	m_size++;
	return index;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
size_t HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::internal_insert(KeyType&& key, const ValueType& value)
{
	const size_t index = find_first_unoccupied_index(Hasher::compute<KeyType>(key) % m_capacity);

	new (&m_key_values[index].key)   KeyType  (Types::move(key));
	new (&m_key_values[index].value) ValueType(value);
	m_states[index] = BucketState::Occupied;

	m_size++;
	return index;
}

template<typename KeyType, typename ValueType, typename AllocatorType, typename Hasher, typename Comparator>
size_t HashTable<KeyType, ValueType, AllocatorType, Hasher, Comparator>::internal_insert(KeyType&& key, ValueType&& value)
{
	const size_t index = find_first_unoccupied_index(Hasher::compute<KeyType>(key) % m_capacity);

	new (&m_key_values[index].key)   KeyType  (Types::move(key));
	new (&m_key_values[index].value) ValueType(Types::move(value));
	m_states[index] = BucketState::Occupied;

	m_size++;
	return index;
}

} // namespace HC