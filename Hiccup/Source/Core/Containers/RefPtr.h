// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/Memory/Memory.h"

#define HC_REFERENCED_COUNTED_OBJECT                    \
	private:                                            \
		::HC::uint64 m_reference_count_internal = 0;    \
		                                                \
		template<typename T>                            \
		friend class ::HC::RefPtr;

namespace HC
{

/**
 *----------------------------------------------------------------
 * Hiccup RefPtr.
 *----------------------------------------------------------------
 * Automatic intrusive reference counted object holder.
 * The instance type must contain 'HC_REFERENCED_COUNTED_OBJECT'
 *   in its declaration.
 */
template<typename T>
class RefPtr
{
public:
	RefPtr()
		: m_instance(nullptr)
	{}

	RefPtr(const RefPtr<T>& other)
		: m_instance(other.m_instance)
	{
		if (m_instance)
		{
			m_instance->m_reference_count_internal++;
		}
	}

	RefPtr(RefPtr<T>&& other) noexcept
		: m_instance(other.m_instance)
	{
		other.m_instance = nullptr;
	}

	explicit RefPtr(T* pointer)
		: m_instance(pointer)
	{
		if (m_instance)
		{
			m_instance->m_reference_count_internal++;
		}
	}

	RefPtr(std::nullptr_t)
		: m_instance(nullptr)
	{}

	~RefPtr()
	{
		release();
	}

	RefPtr<T>& operator=(const RefPtr<T>& other)
	{
		if (other.m_instance)
		{
			other.m_instance->m_reference_count_internal++;
		}

		release();
		m_instance = other.m_instance;
		return *this;
	}

	RefPtr<T>& operator=(RefPtr<T>&& other) noexcept
	{
		m_instance = other.m_instance;
		other.m_instance = nullptr;
		return *this;
	}

	RefPtr<T>& operator=(std::nullptr_t)
	{
		release();
		return *this;
	}

public:
	ALWAYS_INLINE T* operator->() { return get(); }
	ALWAYS_INLINE const T* operator->() const { return get(); }

	ALWAYS_INLINE T& operator*() { return *get(); }
	ALWAYS_INLINE const T& operator*() const { return *get(); }

	ALWAYS_INLINE operator bool() const { return is_valid(); }

public:
	ALWAYS_INLINE T* get()
	{
		HC_DASSERT(m_instance); // Trying to dereference nullptr!
		return m_instance;
	}

	ALWAYS_INLINE const T* get() const
	{
		HC_DASSERT(m_instance); // Trying to dereference nullptr!
		return m_instance;
	}

	ALWAYS_INLINE bool is_valid() const { return (m_instance != nullptr); }

	template<typename Q>
	ALWAYS_INLINE RefPtr<Q> as() const
	{
		return RefPtr<Q>((Q*)m_instance);
	}

	void release()
	{
		if (m_instance)
		{
			if ((--(m_instance->m_reference_count_internal)) == 0)
			{
				hc_delete m_instance;
			}
			m_instance = nullptr;
		}
	}

public:
	ALWAYS_INLINE bool operator==(const RefPtr<T>& other) const
	{
		return (m_instance == other.m_instance);
	}

	ALWAYS_INLINE bool operator!=(const RefPtr<T>& other) const
	{
		return (m_instance != other.m_instance);
	}

public:
	template<typename... Args>
	static RefPtr<T> create(Args&&... args)
	{
		return RefPtr<T>(hc_new T(Types::forward<Args>(args)...));
	}

private:
	// The held object instance.
	T* m_instance;
};

} // namespace HC