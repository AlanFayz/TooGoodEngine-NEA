#pragma once

#include "Core/Base.h"

#include <type_traits>
#include <iterator>

namespace TooGoodEngine {

	/*
		This is designed to be used with a specific type chosen at runtime,
		therefore the type will have to be specified when retreiving an item from memory
		however checks will need to be done to make sure a given memory allocator is using
		the same type
	*/

	static constexpr size_t g_ResizeFactor = 2;

	class MemoryAllocator
	{
	public:
		MemoryAllocator();
		MemoryAllocator(size_t capacity);

		~MemoryAllocator();
		
		inline const void* GetRaw()	const { return m_Buffer; }
		inline const std::string& GetIdentity() const { return m_Identity; }

		//in bytes
		inline const size_t GetCapacity() const { return m_Capacity; }

		//in count of elements
		inline const size_t GetSize()     const	{ return m_Size; }

		template<typename T>
		inline const T& GetElement(size_t index);

		template<typename T>
		inline T* Begin();

		template<typename T>
		inline T* End();

		/*
			Will delete current block of memory then allocate a new one
		*/

		void Allocate(size_t newBytes);

		/*
			Will delete the current block of memory
		*/

		void Deallocate();

		/*
			Will move elements to new buffer with new size then delete old buffer
		*/

		void Reallocate(size_t newBytes);

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

	private:
		void* _Allocate(size_t bytes);
		void  _Deallocate(void* buffer);

	private:
		void* m_Buffer;
		size_t m_Size;
		size_t m_Capacity;

		std::string m_Identity;
	};

	template<typename T>
	inline const T& MemoryAllocator::GetElement(size_t index)
	{
		TGE_VERIFY(index < m_Size && m_Identity == typeid(T).name(), "elements need to be same");

		T* converted = (T*)m_Buffer;

		return converted[index];
	}

	template<typename T>
	inline T* MemoryAllocator::Begin()
	{
		TGE_VERIFY(m_Identity == typeid(T).name(), "elements need to be same");
		return (T*)m_Buffer;
	}

	template<typename T>
	inline T* MemoryAllocator::End()
	{
		TGE_VERIFY(m_Identity == typeid(T).name(), "elements need to be same");
		return (T*)m_Buffer + m_Size;
	}

	template<typename T>
	inline void MemoryAllocator::Insert(const T& element)
	{
		static_assert(std::is_copy_constructible_v<T>, "no copy constructor");

		if (m_Identity.empty())
			m_Identity = typeid(T).name();
		
		TGE_VERIFY(m_Identity == typeid(T).name(), "elements must be the same!");
		
		if (m_Capacity <= m_Size * sizeof(T))
			Reallocate(m_Capacity * g_ResizeFactor + sizeof(T));

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

		if (m_Capacity <= m_Size * sizeof(T))
			Reallocate(m_Capacity * g_ResizeFactor + sizeof(T));

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
}
