#pragma once


#include "Entity.h"

#include <concepts>
#include <vector>
#include <algorithm>
#include <iostream>

namespace TooGoodEngine {

	template<typename Type>
	concept Resizable = std::is_copy_constructible_v<Type> || std::is_move_assignable_v<Type>;

	template<typename Type>
	concept ValidItem = Resizable<Type> && std::is_default_constructible_v<Type>;

	class BaseSparseSet
	{
	public:
		virtual ~BaseSparseSet() = default;

		virtual void Remove(EntityID entityID) = 0;
	};

	template<ValidItem Type>
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

	template<ValidItem Type>
	inline bool SparseSet<Type>::Contains(EntityID entityID) const
	{
		if (m_Sparse.size() > entityID)
			return m_Sparse[entityID] != g_NullEntity;

		return false;
	}

	template<ValidItem Type>
	inline void SparseSet<Type>::Insert(EntityID entityID, const Type& type)
	{
		_ResizeIfNeeded(entityID);
		m_Sparse[entityID] = m_Dense.size();
		m_Dense.push_back(std::make_pair(type, entityID));
	}

	template<ValidItem Type>
	inline void SparseSet<Type>::Remove(EntityID entityID)
	{
		if (entityID >= m_Sparse.size() || m_Sparse[entityID] == g_NullEntity)
			return;

		size_t where = m_Sparse[entityID];

		if (where >= m_Dense.size())
			return;

		m_Dense.erase(m_Dense.begin() + where);

		for (auto& index : m_Sparse)
		{
			if (index != g_NullEntity && index > where)
				index--;
		}

		m_Sparse[entityID] = g_NullEntity;

	}

	//no checks here because assumes the user has done those checks.
	template<ValidItem Type>
	inline Type& SparseSet<Type>::Get(EntityID entityID)
	{
		return m_Dense[m_Sparse[entityID]].first;
	}

	template<ValidItem Type>
	inline void SparseSet<Type>::_ResizeIfNeeded(EntityID entityID)
	{
		if (entityID >= m_Sparse.size())
			m_Sparse.resize(((size_t)entityID + 1) * 2, g_NullEntity);
	}

	template<ValidItem Type>
	template<typename ...Args>
	inline void SparseSet<Type>::Emplace(EntityID entityID, Args&&... args)
	{
		_ResizeIfNeeded(entityID);

		m_Sparse[entityID] = m_Dense.size();
		m_Dense.push_back(std::make_pair(Type(std::forward<Args>(args)...), entityID));
	}
}