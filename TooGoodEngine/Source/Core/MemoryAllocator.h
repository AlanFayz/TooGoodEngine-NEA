#pragma once

#include "Core/Base.h"

#include <type_traits>
#include <iterator>
#include <cstdlib>
#include <xmemory>
#include <vector>

namespace TooGoodEngine {

	inline constexpr size_t g_ResizeFactor = 2;

	template<typename Container>
	class BasicIterator
	{
	public:
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = typename Container::ValueType;
		using pointer = value_type*;
		using reference = value_type&;

		BasicIterator(value_type* ptr, value_type* end) : m_Ptr(ptr), m_Begin(ptr), m_End(end) {}
		BasicIterator(const BasicIterator& other) :
			m_Ptr(other.m_Ptr), m_Begin(other.m_Begin), m_End(other.m_End) {}


		constexpr pointer begin()
		{
			return m_Begin;
		}

		constexpr pointer end()
		{
			return m_End;
		}

		constexpr reference operator*() const noexcept
		{
			TGE_VERIFY(m_Ptr != m_End, "pointer is out of bounds");
			return *m_Ptr;
		}

		constexpr pointer operator->() const noexcept
		{
			return m_Ptr;
		}

		constexpr BasicIterator& operator++() noexcept
		{
			m_Ptr++;
			return *this;
		}

		constexpr BasicIterator operator++(int) noexcept
		{
			BasicIterator iterator = *this;
			++(*this);
			return iterator;
		}

		constexpr BasicIterator& operator--() noexcept
		{
			m_Ptr--;
			return *this;
		}

		constexpr BasicIterator operator--(int) noexcept
		{
			BasicIterator iterator = *this;
			--(*this);
			return iterator;
		}

		constexpr reference operator[](const difference_type _Off) const noexcept
		{
			return *(*this + _Off);
		}

		constexpr bool operator==(const BasicIterator& other) const noexcept
		{
			return m_Ptr == other.m_Ptr;
		}

		constexpr bool operator!=(const BasicIterator& other) const noexcept
		{
			return m_Ptr != other.m_Ptr;
		}

	private:
		value_type* m_Begin;
		value_type* m_Ptr;
		value_type* m_End;
	};

	template<typename Type>
	class MemoryAllocator
	{
	public:
		using ValueType = Type;

	public:
		MemoryAllocator() : m_Buffer(nullptr), m_Capacity(0), m_Size(0)
		{
		}

		~MemoryAllocator()
		{
			for (size_t i = 0; i < m_Capacity; i++)
				m_Buffer[i].~Type();

			if (m_Buffer)
				_Deallocate(m_Buffer);
		}


		inline const void* GetRaw()	const { return m_Buffer; }

		//in bytes
		inline const size_t GetCapacity() const { return m_Capacity; }

		//in count of elements
		inline const size_t GetSize()     const { return m_Size; }

		inline Type& GetElement(size_t index);

		inline Type* Begin();
		inline Type* End();

		/*
			Will delete current block of memory then allocate a new one
		*/

		void Allocate(size_t newCapacity);

		/*
			Will delete the current block of memory
		*/

		void Deallocate();

		/*
			Will move elements to new buffer with new size then delete old buffer
		*/

		void Reallocate(size_t newCapacity);

		/*
			Inserts an element at the end of the buffer
		*/

		void Insert(const Type& element);

		/*
			Inserts an array of elements
		*/

		template<typename Iterator>
		void InsertN(Iterator begin, Iterator end);

		/*
			construts an element at the end of the buffer,
			must have a valid constructor
		*/

		template<typename ...Args>
		void Emplace(Args&&... args);

		/*
			Will remove an element at any index and must have a valid move constructor.
			Note: this is done by swapping the last element with
			the element at the index so make sure to update any data
			accordingly
		*/

		void Remove(size_t index);


		BasicIterator<MemoryAllocator<Type>> View()
		{
			return BasicIterator<MemoryAllocator<Type>>(Begin(), End());
		}

	private:
		Type* _Allocate(size_t nElements);
		void  _Deallocate(void* buffer);
		Type* _Reallocate(size_t nElements);

	private:
		Type* m_Buffer;
		size_t m_Size;
		size_t m_Capacity;

	};

	template<typename Type>
	inline Type& MemoryAllocator<Type>::GetElement(size_t index)
	{
		TGE_VERIFY(index < m_Size, "index out of range");
		return m_Buffer[index];
	}

	template<typename Type>
	inline Type* MemoryAllocator<Type>::Begin()
	{
		return m_Buffer;
	}

	template<typename Type>
	inline Type* MemoryAllocator<Type>::End()
	{
		return m_Buffer + m_Size;
	}


	template<typename Type>
	inline void MemoryAllocator<Type>::Allocate(size_t newCapacity)
	{
		if (m_Buffer)
			Deallocate();

		m_Buffer = _Allocate(newCapacity);
		m_Capacity = newCapacity;

		for (size_t i = 0; i < m_Capacity; i++)
			new(m_Buffer + i) Type();
	}

	template<typename Type>
	inline void MemoryAllocator<Type>::Deallocate()
	{
		for (size_t i = 0; i < m_Capacity; ++i)
			m_Buffer[i].~Type();

		_Deallocate(m_Buffer);
		m_Buffer = nullptr;
	}

	template<typename Type>
	inline void MemoryAllocator<Type>::Reallocate(size_t newCapacity)
	{
		//if buffer is already null then we can just allocate memory normally
		if (!m_Buffer)
		{
			Allocate(newCapacity);
			return;
		}

		if (newCapacity < m_Capacity)
		{
			for (size_t i = newCapacity; i < m_Capacity; i++)
				m_Buffer[i].~Type();
		}

		m_Buffer = _Reallocate(newCapacity);

		if (newCapacity > m_Capacity)
		{
			for (size_t i = m_Capacity; i < newCapacity; i++)
				new(m_Buffer + i) Type();
		}

		m_Capacity = newCapacity;
	}

	template<typename Type>
	inline void MemoryAllocator<Type>::Insert(const Type& element)
	{
		static_assert(std::is_copy_constructible_v<Type>, "no copy constructor");

		if (m_Capacity <= m_Size)
			Reallocate((m_Size + 1) * g_ResizeFactor);

		m_Buffer[m_Size++] = element;
	}


	template<typename Type>
	template<typename Iterator>
	void MemoryAllocator<Type>::InsertN(Iterator begin, Iterator end)
	{
		using value_type = typename std::iterator_traits<Iterator>::value_type;
		static_assert(std::is_same_v<value_type, ValueType>, "not the correct type");

		for (Iterator iterator = begin; iterator != end; iterator++)
			Insert(*iterator);
	}

	template<typename Type>
	template<typename ...Args>
	inline void MemoryAllocator<Type>::Emplace(Args&& ...args)
	{
		static_assert(std::is_constructible_v<Type>, "no constructor");

		if (m_Capacity <= m_Size)
			Reallocate((m_Size + 1) * g_ResizeFactor);

		m_Buffer[m_Size++] = Type(std::forward<Args>(args)...);
	}

	template<typename Type>
	inline void MemoryAllocator<Type>::Remove(size_t index)
	{
		TGE_VERIFY(m_Buffer, "buffer is null");
		TGE_VERIFY(index < m_Size, "index out of range");


		Type* toDelete = m_Buffer + index;
		Type* end = m_Buffer + m_Size - 1;

		(*toDelete).~Type();

		std::swap(*toDelete, *end);
		m_Size--;
	}
	template<typename Type>
	inline Type* MemoryAllocator<Type>::_Allocate(size_t nElements)
	{
		//using windows specific until std::aligned_alloc becomes available on msvc (may add a compile time check)
		return (Type*)_aligned_malloc(nElements * sizeof(Type), alignof(Type));
	}
	template<typename Type>
	inline void MemoryAllocator<Type>::_Deallocate(void* buffer)
	{
		_aligned_free(buffer);
	}
	template<typename Type>
	inline Type* MemoryAllocator<Type>::_Reallocate(size_t nElements)
	{
		return (Type*)_aligned_realloc(m_Buffer, nElements * sizeof(Type), alignof(Type));
	}
}
