#pragma once

#include "MemoryAllocator.h"
#include "Entity.h"

#include <vector>
#include <unordered_map>

namespace TooGoodEngine {

	class DenseMap
	{
	public:
		DenseMap()  = default;
		~DenseMap() = default;

		const std::string& GetIdentity() const { return m_Dense.GetIdentity(); }

		template<typename T>
		inline const bool Contains(EntityID entity) const 
		{ 
			TGE_VERIFY(GetIdentity() == typeid(T).name(), "not the same identity");
			return m_Sparse.contains(entity); 
		}

		template<typename T>
		inline T* BeginDense() { return m_Dense.Begin<T>(); }

		template<typename T>
		inline T* EndDense() { return m_Dense.End<T>();  };

		template<typename T>
		inline MemoryAllocator::VariableIterator<T> ViewDense() { return m_Dense.View<T>(); }

		template<typename T> 
		void Add(EntityID entity, const T& type);

		template<typename T, typename ...Args>
		void Emplace(EntityID entity, Args&&... args);

		template<typename T>
		void Remove(EntityID entity);

		template<typename T>
		T& Get(EntityID entity);

		/*
			Function should take in the type of component as a reference
			and a second paramater for the EntityID.
		*/

		template<typename T, typename Fun>
		void ForEach(Fun fun);

	private:
		std::unordered_map<EntityID, size_t> m_Sparse;
		std::unordered_map<size_t, EntityID> m_IndexToEntity;
		MemoryAllocator m_Dense;
	};

	template<typename T>
	inline void DenseMap::Add(EntityID entity, const T& type)
	{
		TGE_VERIFY(!m_Sparse.contains(entity), "entity already contains that component");

		m_Dense.Insert<T>(type);
		m_Sparse[entity] = m_Dense.GetSize() - 1;
		m_IndexToEntity[m_Dense.GetSize() - 1] = entity;
	}

	template<typename T, typename ...Args>
	inline void DenseMap::Emplace(EntityID entity, Args&&... args)
	{
		TGE_VERIFY(!m_Sparse.contains(entity), "entity already contains that component");

		m_Dense.Emplace<T>(std::forward<Args>(args)...);
		m_Sparse[entity] = m_Dense.GetSize() - 1;
		m_IndexToEntity[m_Dense.GetSize() - 1] = entity;
	}

	template<typename T>
	inline void DenseMap::Remove(EntityID entity)
	{
		TGE_VERIFY(m_Sparse.contains(entity), "entity doesn't contain that component");

		size_t index  = m_Dense.GetSize() - 1;
		size_t at     = m_Sparse[entity];
		EntityID last = m_IndexToEntity[index];

		m_Dense.Remove<T>(at);
		m_Sparse.erase(entity);
		m_IndexToEntity.erase(index);

		m_Sparse[last] = at;
		m_IndexToEntity[at] = last;
	}

	template<typename T>
	inline T& DenseMap::Get(EntityID entity)
	{
		TGE_VERIFY(m_Sparse.contains(entity), "entity has no component");
		return m_Dense.GetElement<T>(m_Sparse[entity]);
	}

	template<typename T, typename Fun>
	inline void DenseMap::ForEach(Fun fun)
	{
		T* begin = m_Dense.Begin<T>();
		T* end   = m_Dense.End<T>();

		size_t k = 0;
		for (T* it = begin; it != end; it++)
			fun(*it, m_IndexToEntity[k++]);
	}

}

