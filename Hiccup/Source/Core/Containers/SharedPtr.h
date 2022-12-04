// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/Memory/Memory.h"

#define HC_REFERENCED_COUNTED_OBJECT            \
	private:                                    \
		uint64 m_reference_count_internal = 0;  \
		                                        \
		template<typename T>                    \
		friend class SharedPtr;

namespace HC
{

/**
 *----------------------------------------------------------------
 * Hiccup SharedPtr.
 *----------------------------------------------------------------
 * Automatic intrusive reference counted object holder.
 * The instance type must contain 'HC_REFERENCED_COUNTED_OBJECT'
 *   in its declaration.
 */
template<typename T>
class SharedPtr
{
public:
	/**
	 * Default constructor.
	 */
	SharedPtr();

	/**
	 * Copy constructor.
	 * 
	 * @param other The shared pointer to copy.
	 */
	SharedPtr(const SharedPtr<T>& other);

	/**
	 * Move constructor.
	 * 
	 * @param other The shared pointer to move.
	 */
	SharedPtr(SharedPtr<T>&& other) noexcept;

	/**
	 * Constructs a shared pointer from a raw pointer.
	 * 
	 * @param pointer The raw pointer to create the shared pointer from.
	 */
	explicit SharedPtr(T* pointer);

	/**
	 * Constructs a shared pointer from nullptr. The shared pointer is
	 *   initialized to its invalid state.
	 */
	SharedPtr(std::nullptr_t);

	/**
	 * Destructor.
	 */
	~SharedPtr();

	/**
	 * Copy assignment operator.
	 * 
	 * @param other The shared pointer to copy.
	 * 
	 * @return Reference to this, after the copy.
	 */
	SharedPtr<T>& operator=(const SharedPtr<T>& other);

	/**
	 * Move assignment operator.
	 *
	 * @param other The shared pointer to move.
	 *
	 * @return Reference to this, after the move.
	 */
	SharedPtr<T>& operator=(SharedPtr<T>&& other) noexcept;

	/**
	 * Assignment operator using nullptr. The shared pointer is
	 *   set to its invalid state.
	 * 
	 * @return Reference to this, after the invalidation.
	 */
	SharedPtr<T>& operator=(std::nullptr_t);

public:
	/** @return The held object. */
	ALWAYS_INLINE T* operator->();

	/** @return The held object. */
	ALWAYS_INLINE const T* operator->() const;

public:
	/** @return The held object. */
	ALWAYS_INLINE T* get();

	/** @return The held object. */
	ALWAYS_INLINE const T* get() const;

	/** @return True if the pointer is valid (not nullptr); False otherwise. */
	ALWAYS_INLINE bool is_valid() const;

public:
	/**
	 * Equality check operator.
	 * 
	 * @param other The shared pointer to compare against.
	 * 
	 * @return True if the pointers are equal; False otherwise.
	 */
	bool operator==(const SharedPtr<T>& other) const;

	/**
	 * Inequality check operator.
	 *
	 * @param other The shared pointer to compare against.
	 *
	 * @return True if the pointers are not equal; False otherwise.
	 */
	bool operator!=(const SharedPtr<T>& other) const;

	/** @return True if the shared pointer is valid; False otherwise. */
	operator bool() const;

public:
	/**
	 * Releases the object, decrementing its reference count. If the count hits 0,
	 *   the object destructor is called and the memory is freed.
	 */
	void release();

private:
	// The held object.
	T* m_instance;
};

//////////////// SHARED POINTER IMPLEMENTATION ////////////////

template<typename T>
SharedPtr<T>::SharedPtr()
	: m_instance(nullptr)
{}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& other)
	: m_instance(other.m_instance)
{
	if (m_instance)
	{
		m_instance->m_reference_count_internal++;
	}
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& other) noexcept
	: m_instance(other.m_instance)
{
	other.m_instance = nullptr;
}

template<typename T>
SharedPtr<T>::SharedPtr(T* pointer)
	: m_instance(pointer)
{
	if (m_instance)
	{
		m_instance->m_reference_count_internal++;
	}
}

template<typename T>
SharedPtr<T>::SharedPtr(std::nullptr_t)
	: m_instance(nullptr)
{}

template<typename T>
SharedPtr<T>::~SharedPtr()
{
	release();
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other)
{
	if (other.m_instance)
	{
		other.m_instance->m_reference_count_internal++;
	}

	release();
	m_instance = other.m_instance;

	return *this;
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& other) noexcept
{
	m_instance = other.m_instance;
	other.m_instance = nullptr;
	return *this;
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(std::nullptr_t)
{
	release();
	return *this;
}

template<typename T>
ALWAYS_INLINE T* SharedPtr<T>::operator->()
{
	return get();
}

template<typename T>
ALWAYS_INLINE const T* SharedPtr<T>::operator->() const
{
	return get();
}

template<typename T>
ALWAYS_INLINE T* SharedPtr<T>::get()
{
	HC_ASSERT(m_instance != nullptr); // Trying to dereference a nullptr!
	return m_instance;
}

template<typename T>
ALWAYS_INLINE const T* SharedPtr<T>::get() const
{
	HC_ASSERT(m_instance != nullptr); // Trying to dereference a nullptr!
	return m_instance;
}

template<typename T>
ALWAYS_INLINE bool SharedPtr<T>::is_valid() const
{
	return (m_instance != nullptr);
}

template<typename T>
bool SharedPtr<T>::operator==(const SharedPtr<T>& other) const
{
	return m_instance == other.m_instance;
}

template<typename T>
bool SharedPtr<T>::operator!=(const SharedPtr<T>& other) const
{
	return m_instance != other.m_instance;
}

template<typename T>
SharedPtr<T>::operator bool() const
{
	return is_valid();
}

template<typename T>
void SharedPtr<T>::release()
{
	if (!m_instance)
	{
		return;
	}

	if ((--(m_instance->m_reference_count_internal)) == 0)
	{
		hc_delete m_instance;
	}

	m_instance = nullptr;
}

} // namespace HC