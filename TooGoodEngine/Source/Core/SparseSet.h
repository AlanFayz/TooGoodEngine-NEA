#pragma once

#include <concepts>
#include <vector>
#include <algorithm>
#include <iostream>

namespace TooGoodEngine {

	template<typename Type>
	concept Resizable = (std::is_copy_constructible_v<Type> || std::is_move_assignable_v<Type>) && std::is_default_constructible_v<Type>;

	class BaseSparseSet
	{
	public:
		virtual ~BaseSparseSet() = default;

		virtual void Remove(size_t index) = 0;
	};

	//if needed may add the ability to have a custom sparse allocator as well but don't see any benfit currently

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

		auto begin() const { return m_Dense.begin(); }
		auto end()   const { return m_Dense.end(); }

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

		size_t where = m_Sparse[index];

		if (where >= m_Dense.size())
			return;   

		size_t lastIndex = m_Dense.size() - 1;

		if (where != lastIndex)
		{
			size_t lastSparseIndex = m_Dense[lastIndex].second;

			std::swap(m_Dense[where], m_Dense[lastIndex]);

			m_Sparse[lastSparseIndex] = where;
		}

		m_Dense.pop_back();

		m_Sparse[index] = SparseNull;
	}

	//no checks here because assumes the user has done those checks.
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