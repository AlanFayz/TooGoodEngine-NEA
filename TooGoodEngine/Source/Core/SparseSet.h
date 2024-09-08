#pragma once

#include <concepts>
#include <vector>
#include <algorithm>

namespace TooGoodEngine {
	
	//makes sure that any type entered into the sparse set has these properties (allows the STL vector to resize)
	//otherwise there will be a lot of compile time errors that are hard to track down.
	//this makes error checking easier.
	template<typename Type>
	concept Resizable =  (std::is_copy_constructible_v<Type> || std::is_move_constructible_v<Type>) && 
						  std::is_move_assignable_v<Type> &&
						  std::is_default_constructible_v<Type>;

	//we have a base so that we can easily remove at an index without any type information.
	class BaseSparseSet
	{
	public:
		virtual ~BaseSparseSet() = default;

		virtual void Remove(size_t index) = 0;
	};
	
	//A sparse set is a data structure which has 2 vectors a sparse and a dense
	// 			(0)     (1)     (2)     (3)  
	// DENSE:	Data	Data	Data	Data
	// Holds the actual data being stored
	// 
	// SPARSE:  0	 1	 2	 4	 3  
	// the sparse vector holds the index of the element its pointing to in the dense array
	// a sparse vector is "sparse" it can have empty elements in between elements that are occupied.
	// It essentially acts as a map but reduces the overhead of hashing. Pointers can also be used
	// directly but this is not cache friendly and can cost significant performance. I have personally
	// benchmarked this with my entity componenet system. with trying to render 100,000 entities by default with an 
	// unoredered_map was around 300-600ms per frame. switching to this data structure it was reduced to 2-5ms per frame!
	// however this does waste memory if there are many empty slots but this shouldn't effect performance as the dense
	// array is packed together.
	
	//a custom allocator and default sparse null value is also able to be provided given that the allocator
	//follows the stl standard
	template<Resizable Type, size_t SparseNull = std::numeric_limits<size_t>::max(), class Allocator = std::allocator<Type>>
	class SparseSet : public BaseSparseSet
	{
	public:
		using ValueType     = std::pair<Type, size_t>;
		using AllocatorType = std::allocator_traits<Allocator>::template rebind_alloc<ValueType>;
		using SparseStorage = std::vector<size_t>;
		using DenseStorage  = std::vector<ValueType,  AllocatorType>;

	public:
		SparseSet() = default;
		virtual ~SparseSet() = default;

		inline bool Contains(size_t index) const;

		void Insert(size_t index, const Type& type);

		template<typename ...Args>
		void Emplace(size_t index, Args&&... args);

		virtual void Remove(size_t index) override;

		inline Type& Get(size_t index);

		inline const SparseStorage& GetSparse() const { return m_Sparse; }
		inline const DenseStorage&  GetDense()  const { return m_Dense; }
		inline const AllocatorType GetDenseAllocator() const { return m_Dense.get_allocator(); }

		auto begin() { return m_Dense.begin(); }
		auto end()   { return m_Dense.end(); }

	private:
		void _ResizeIfNeeded(size_t index);

	private:
		SparseStorage m_Sparse; //indices
		DenseStorage  m_Dense;  //elements
	};

	template<Resizable Type, size_t SparseNull, class Allocator>
	inline bool SparseSet<Type, SparseNull, Allocator>::Contains(size_t index) const
	{
		if (m_Sparse.size() > index)
			return m_Sparse[index] != SparseNull;

		return false;
	}

	template<Resizable Type, size_t SparseNull, class Allocator>
	inline void SparseSet<Type, SparseNull, Allocator>::Insert(size_t index, const Type& type)
	{
		_ResizeIfNeeded(index);
		m_Sparse[index] = m_Dense.size();
		m_Dense.push_back(std::make_pair(type, index));
	}

	template<Resizable Type, size_t SparseNull, class Allocator>
	inline void SparseSet<Type, SparseNull, Allocator>::Remove(size_t index)
	{
		if (index >= m_Sparse.size() || m_Sparse[index] == SparseNull)
			return;

		//get the index of what needs to be deleted
		size_t where = m_Sparse[index];

		if (where >= m_Dense.size())
			return;   

		//get the last index as well
		size_t lastIndex = m_Dense.size() - 1;

		//if where and lastIndex are different then we swap them
		//and adjust the sparse vector accordingly.
		if (where != lastIndex)
		{
			size_t lastSparseIndex = m_Dense[lastIndex].second;

			std::swap(m_Dense[where], m_Dense[lastIndex]);

			m_Sparse[lastSparseIndex] = where;
		}

		//delete the element at the top of the vector.
		m_Dense.pop_back();

		//assign that index to null
		m_Sparse[index] = SparseNull;
	}

	//no checks here, assumes the user has done those checks.
	template<Resizable Type, size_t SparseNull, class Allocator>
	inline Type& SparseSet<Type, SparseNull, Allocator>::Get(size_t index)
	{
		return m_Dense[m_Sparse[index]].first;
	}

	template<Resizable Type, size_t SparseNull, class Allocator>
	inline void SparseSet<Type, SparseNull, Allocator>::_ResizeIfNeeded(size_t index)
	{
		if (index >= m_Sparse.size())
			m_Sparse.resize((index + 1) * 2, SparseNull);
	}

	template<Resizable Type, size_t SparseNull, class Allocator>
	template<typename ...Args>
	inline void SparseSet<Type, SparseNull, Allocator>::Emplace(size_t index, Args&&... args)
	{
		_ResizeIfNeeded(index);

		m_Sparse[index] = m_Dense.size();
		m_Dense.push_back(std::make_pair(Type(std::forward<Args>(args)...), index));
	}
}