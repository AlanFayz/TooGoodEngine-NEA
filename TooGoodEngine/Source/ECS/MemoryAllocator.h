#pragma once

#include "Core/Base.h"

#include <type_traits>
#include <iterator>
#include <cstdlib>
#include <xmemory>
#include <vector>

namespace TooGoodEngine {

	/*
		This is designed to be used with a specific type chosen at runtime,
		therefore the type will have to be specified when retreiving an item from memory
		however checks will need to be done to make sure a given memory allocator is using
		the same type
	*/

	inline constexpr size_t g_ResizeFactor = 2;

	class MemoryAllocator
	{
	public:
		MemoryAllocator();

		~MemoryAllocator();
		
		inline const void* GetRaw()	const { return m_Buffer; }
		inline const std::string& GetIdentity() const { return m_Identity; }

		//in bytes
		inline const size_t GetCapacity() const { return m_Capacity; }

		//in count of elements
		inline const size_t GetSize()     const	{ return m_Size; }

		template<typename T>
		inline T& GetElement(size_t index);

		template<typename T>
		inline T* Begin();

		template<typename T>
		inline T* End();

		/*
			Will delete current block of memory then allocate a new one
		*/

		template<typename T>
		void Allocate(size_t newCapacity);

		/*
			Will delete the current block of memory
		*/

		template<typename T>
		void Deallocate();

		/*
			Will move elements to new buffer with new size then delete old buffer
		*/

		template<typename T>
		void Reallocate(size_t newCapacity);

		/*
			Inserts an element at the end of the buffer
		*/

		template<typename T>
		void Insert(const T& element);

		/*
			Inserts an array of elements
		*/

		template<typename Iterator>
		void InsertN(Iterator begin, Iterator end);

		/*
			construts an element at the end of the buffer,
			must have a valid constructor
		*/

		template<typename T, typename ...Args>
		void Emplace(Args&&... args);

		/*
			Will remove an element at any index and must have a valid move constructor. 
			Note: this is done by swapping the last element with
			the element at the index so make sure to update any data 
			accordingly
		*/

		template<typename T>
		void Remove(size_t index);

		template<typename Type>
		class VariableIterator
		{
		public:
			using iterator_category = std::random_access_iterator_tag;
			using difference_type	= std::ptrdiff_t;
			using value_type		= Type;
			using pointer			= value_type*;
			using reference			= value_type&;

			VariableIterator(Type* ptr, Type* end) : m_Ptr(ptr), m_Begin(ptr), m_End(end) {}
			VariableIterator(const VariableIterator& other) :
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

			constexpr VariableIterator& operator++() noexcept
			{
				m_Ptr++;
				return *this;
			}

			constexpr VariableIterator operator++(int) noexcept
			{
				VariableIterator iterator = *this;
				++(*this);
				return iterator;
			}

			constexpr VariableIterator& operator--() noexcept
			{
				m_Ptr--;
				return *this;
			}

			constexpr VariableIterator operator--(int) noexcept
			{
				VariableIterator iterator = *this;
				--(*this);
				return iterator;
			}

			constexpr reference operator[](const difference_type _Off) const noexcept 
			{
				return *(*this + _Off);
			}

			constexpr bool operator==(const VariableIterator& other) const noexcept
			{
				return m_Ptr == other.m_Ptr;
			}

			constexpr bool operator!=(const VariableIterator& other) const noexcept
			{
				return m_Ptr != other.m_Ptr;
			}

		private:
			Type* m_Begin;
			Type* m_Ptr;
			Type* m_End;
		};


		template<typename T>
		VariableIterator<T> View()
		{
			TGE_VERIFY(m_Identity == typeid(T).name(), "not the same identity");
			return VariableIterator<T>(Begin<T>(), End<T>());
		}

	private:
		template<typename T>
		void* _Allocate(size_t nElements);
		void  _Deallocate(void* buffer);

		template<typename T>
		void* _Reallocate(size_t nElements);

	private:
		void* m_Buffer;
		size_t m_Size;
		size_t m_Capacity;

		std::string m_Identity;
	};

	template<typename T>
	inline T& MemoryAllocator::GetElement(size_t index)
	{
		TGE_VERIFY(index < m_Size && m_Identity == typeid(T).name(), "elements need to be same");

		T* converted = (T*)m_Buffer;

		return converted[index];
	}

	template<typename T>
	inline T* MemoryAllocator::Begin()
	{
		if (m_Identity.empty())
			return nullptr;

		TGE_VERIFY(m_Identity == typeid(T).name(), "elements need to be same");
		return (T*)m_Buffer;
	}

	template<typename T>
	inline T* MemoryAllocator::End()
	{
		if (m_Identity.empty())
			return nullptr;

		TGE_VERIFY(m_Identity == typeid(T).name(), "elements need to be same");
		return (T*)m_Buffer + m_Size;
	}

	template<typename T>
	inline void MemoryAllocator::Allocate(size_t newCapacity)
	{
		if (m_Buffer)
			Deallocate<T>();

		m_Buffer = _Allocate<T>(newCapacity);
		m_Capacity = newCapacity;

		for(size_t i = 0; i < m_Capacity; i++)
			new((T*)m_Buffer + i) T();
	}

	template<typename T>
	inline void MemoryAllocator::Deallocate()
	{
		for (size_t i = 0; i < m_Capacity; ++i) 
			((T*)m_Buffer)[i].~T();
		
		_Deallocate(m_Buffer);
		m_Buffer = nullptr;
	}

	template<typename T>
	inline void MemoryAllocator::Reallocate(size_t newCapacity)
	{
		//if buffer is already null then we can just allocate memory normally
		if (!m_Buffer)
		{
			Allocate<T>(newCapacity);
			return;
		}

		if (newCapacity < m_Capacity)
		{
			for (size_t i = newCapacity; i < m_Capacity; i++)
				((T*)m_Buffer)[i].~T();
		}

		m_Buffer = _Reallocate<T>(newCapacity);


		if (newCapacity > m_Capacity)
		{
			for (size_t i = m_Capacity; i < newCapacity; i++)
				new((T*)m_Buffer + i) T();
		}
		 
		m_Capacity = newCapacity;
	}

	template<typename T>
	inline void MemoryAllocator::Insert(const T& element)
	{
		static_assert(std::is_copy_constructible_v<T>, "no copy constructor");

		if (m_Identity.empty())
			m_Identity = typeid(T).name();
		
		TGE_VERIFY(m_Identity == typeid(T).name(), "elements must be the same!");
		
		if (m_Capacity <= m_Size)
			Reallocate<T>((m_Size + 1) * g_ResizeFactor);


		T* converted = (T*)m_Buffer; 
		converted[m_Size++] = element;
	}

	template<typename Iterator>
	inline void MemoryAllocator::InsertN(Iterator begin, Iterator end)
	{
		TGE_VERIFY(end > begin, "cannot be the same iterator");

		using ValueType = typename std::iterator_traits<Iterator>::value_type;

		for (Iterator iterator = begin; iterator != end; iterator++)
			Insert<ValueType>(*iterator);

	}

	template<typename T, typename ...Args>
	inline void MemoryAllocator::Emplace(Args && ...args)
	{
		static_assert(std::is_constructible_v<T>, "no constructor");

		if (m_Identity.empty())
			m_Identity = typeid(T).name();

		TGE_VERIFY(m_Identity == typeid(T).name(), "elements must be the same!");

		if (m_Capacity <= m_Size)
			Reallocate<T>((m_Size + 1) * g_ResizeFactor);

		T* converted = (T*)(m_Buffer);
		converted[m_Size++] = T(std::forward<Args>(args)...);
	}

	template<typename T>
	inline void MemoryAllocator::Remove(size_t index)
	{
		TGE_VERIFY(m_Buffer, "buffer is null");

		if (m_Identity.empty())
			m_Identity = typeid(T).name();

		TGE_VERIFY(m_Identity == typeid(T).name() &&
				   index < m_Size, 
				   "elements must be the same or index out of range");


		T* toDelete = (T*)m_Buffer + index;
		T* end = (T*)m_Buffer + m_Size - 1;

		(*toDelete).~T();

		std::swap(*toDelete, *end);
		m_Size--;
	}
	template<typename T>
	inline void* MemoryAllocator::_Allocate(size_t nElements)
	{
		//using windows specific until std::aligned_alloc becomes available on msvc (may add a compile time check)
		return _aligned_malloc(nElements * sizeof(T), alignof(T));
	}
	template<typename T>
	inline void* MemoryAllocator::_Reallocate(size_t nElements)
	{
		return _aligned_realloc(m_Buffer, nElements * sizeof(T), alignof(T));
	}
}
