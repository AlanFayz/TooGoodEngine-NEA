#pragma once


#include "Entity.h"

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

		virtual void Remove(EntityID entityID) = 0;
	};

	template<Resizable Type>
	class SparseSet : public BaseSparseSet
	{
	public:
		using ValueType     = std::pair<Type, size_t>;
		using SparseStorage = std::vector<size_t>;
		using DenseStorage  = std::vector<ValueType>;

	public:
		SparseSet() = default;
		virtual ~SparseSet() = default;

		inline bool Contains(EntityID entityID) const;

		void Insert(EntityID entityID, const Type& type);

		template<typename ...Args>
		void Emplace(EntityID entityID, Args&&... args);

		virtual void Remove(EntityID entityID) override;

		inline Type& Get(EntityID entityID);

		inline const SparseStorage& GetSparse() const { return m_Sparse; }
		inline const DenseStorage& GetDense()  const { return m_Dense; }

		auto begin() const { return m_Dense.begin(); }
		auto end()   const { return m_Dense.end(); }

	private:
		void _ResizeIfNeeded(EntityID entityID);

	private:
		SparseStorage m_Sparse; //indices
		DenseStorage  m_Dense;  //elements
	};

	template<Resizable Type>
	inline bool SparseSet<Type>::Contains(EntityID entityID) const
	{
		if (m_Sparse.size() > entityID)
			return m_Sparse[entityID] != g_NullEntity;

		return false;
	}

	template<Resizable Type>
	inline void SparseSet<Type>::Insert(EntityID entityID, const Type& type)
	{
		_ResizeIfNeeded(entityID);
		m_Sparse[entityID] = m_Dense.size();
		m_Dense.push_back(std::make_pair(type, entityID));
	}

	template<Resizable Type>
	inline void SparseSet<Type>::Remove(EntityID entityID)
	{
		if (entityID >= m_Sparse.size() || m_Sparse[entityID] == g_NullEntity)
			return;

		size_t where = m_Sparse[entityID];

		if (where >= m_Dense.size())
			return;   

		size_t lastIndex = m_Dense.size() - 1;

		if (where != lastIndex)
		{
			EntityID lastEntityID = m_Dense[lastIndex].second;

			std::swap(m_Dense[where], m_Dense[lastIndex]);

			m_Sparse[lastEntityID] = where;
		}

		m_Dense.pop_back();

		m_Sparse[entityID] = g_NullEntity;
	}

	//no checks here because assumes the user has done those checks.
	template<Resizable Type>
	inline Type& SparseSet<Type>::Get(EntityID entityID)
	{
		return m_Dense[m_Sparse[entityID]].first;
	}

	template<Resizable Type>
	inline void SparseSet<Type>::_ResizeIfNeeded(EntityID entityID)
	{
		if (entityID >= m_Sparse.size())
			m_Sparse.resize((entityID + 1) * 2, g_NullEntity);
	}

	template<Resizable Type>
	template<typename ...Args>
	inline void SparseSet<Type>::Emplace(EntityID entityID, Args&&... args)
	{
		_ResizeIfNeeded(entityID);

		m_Sparse[entityID] = m_Dense.size();
		m_Dense.push_back(std::make_pair(Type(std::forward<Args>(args)...), entityID));
	}
}