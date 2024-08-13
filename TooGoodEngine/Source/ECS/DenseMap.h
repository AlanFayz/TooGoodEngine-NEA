#pragma once

#include "MemoryAllocator.h"
#include "Entity.h"

#include <vector>
#include <unordered_map>

namespace TooGoodEngine {

	class BaseDenseMap
	{
	public:
		virtual ~BaseDenseMap() = default;
	};

	template<typename Type>
	class DenseMap : public BaseDenseMap
	{
	public:
		DenseMap()  = default;
		virtual ~DenseMap() = default;

		inline const bool Contains(EntityID entity) const 
		{ 
			return m_Sparse.contains(entity); 
		}

		inline Type* BeginDense() { return m_Dense.Begin(); }
		inline Type* EndDense()   { return m_Dense.End();  };

		inline auto ViewDense() { return m_Dense.View(); }

		void Add(EntityID entity, const Type& type);

		template<typename ...Args>
		void Emplace(EntityID entity, Args&&... args);

		void Remove(EntityID entity);

		Type& Get(EntityID entity);

		/*
			Function should take in the type of component as a reference
			and a second paramater for the EntityID.
		*/

		template<typename Fun>
		void ForEach(Fun fun);

	private:
		std::unordered_map<EntityID, size_t> m_Sparse;
		std::unordered_map<size_t, EntityID> m_IndexToEntity;
		MemoryAllocator<Type> m_Dense;
	};


	template<typename Type>
	inline void DenseMap<Type>::Add(EntityID entity, const Type& type)
	{
		TGE_VERIFY(!m_Sparse.contains(entity), "entity already contains that component");

		m_Dense.Insert(type);
		m_Sparse[entity] = m_Dense.GetSize() - 1;
		m_IndexToEntity[m_Dense.GetSize() - 1] = entity;
	}

	template<typename Type>
	template<typename ...Args>
	inline void DenseMap<Type>::Emplace(EntityID entity, Args&&... args)
	{
		TGE_VERIFY(!m_Sparse.contains(entity), "entity already contains that component");

		m_Dense.Emplace(std::forward<Args>(args)...);
		m_Sparse[entity] = m_Dense.GetSize() - 1;
		m_IndexToEntity[m_Dense.GetSize() - 1] = entity;
	}

	template<typename Type>
	inline void DenseMap<Type>::Remove(EntityID entity)
	{
		TGE_VERIFY(m_Sparse.contains(entity), "entity doesn't contain that component");

		size_t index  = m_Dense.GetSize() - 1;
		size_t at     = m_Sparse[entity];
		EntityID last = m_IndexToEntity[index];

		m_Dense.Remove(at);
		m_Sparse.erase(entity);
		m_IndexToEntity.erase(index);

		m_Sparse[last] = at;
		m_IndexToEntity[at] = last;
	}

	template<typename Type>
	inline Type& DenseMap<Type>::Get(EntityID entity)
	{
		TGE_VERIFY(m_Sparse.contains(entity), "entity has no component");
		return m_Dense.GetElement(m_Sparse[entity]);
	}

	template<typename Type> 
	template<typename Fun>
	inline void DenseMap<Type>::ForEach(Fun fun)
	{
		Type* begin = m_Dense.Begin();
		Type* end   = m_Dense.End();

		size_t k = 0;
		for (Type* it = begin; it != end; it++)
			fun(*it, m_IndexToEntity[k++]);
	}

}

