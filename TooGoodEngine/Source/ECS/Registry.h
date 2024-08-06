#pragma once

#include "DenseMap.h"
#include "Entity.h"

namespace TooGoodEngine {

	class Registry
	{
	public:
		Registry() = default;
		~Registry() = default;

		inline Entity CreateEntity(const std::string& name) 
		{
			Entity entity = Entity(name, m_Count++);
			m_Entites.push_back(entity);
			return entity; 
		}
		inline const size_t GetCount() const { return (size_t)m_Count; }

		inline Entity GetEntity(EntityID id)
		{
			if (id < m_Entites.size())
				return m_Entites[id];

			return Entity("null entity", g_NullEntity);
		}
		
		template<typename T>
		inline bool HasComponent(const EntityID& entity) 
		{ 
			if(m_Buckets.contains(typeid(T).name()))
				return m_Buckets[typeid(T).name()].Contains<T>(entity);

			return false;
		}
	
		template<typename T>
		void AddComponent(const Entity& entity, const T& t);

		template<typename T, typename ...Args>
		void EmplaceComponent(const Entity& entity, Args&&... args);

		template<typename T>
		void RemoveComponent(const Entity& entity);

		template<typename T>
		T& GetComponent(const EntityID& entity);

		template<typename T, typename Fun>
		void ForEach(Fun fun);

		template<typename T>
		MemoryAllocator::VariableIterator<T> View();

	private:
		void _VerifyEntity(const EntityID& entity) const;

	private:
		EntityID m_Count = 0;
		std::unordered_map<std::string, DenseMap> m_Buckets; //string identifier
		std::vector<Entity> m_Entites;
	};


	template<typename T>
	inline void Registry::AddComponent(const Entity& entity, const T& t)
	{
		_VerifyEntity(entity);
		m_Buckets[typeid(T).name()].Add<T>(entity, t);
	}

	template<typename T, typename ...Args>
	inline void Registry::EmplaceComponent(const Entity& entity, Args&&... args)
	{
		_VerifyEntity(entity);
		m_Buckets[typeid(T).name()].Emplace<T>(entity, std::forward<Args>(args)...);
	}

	template<typename T>
	inline void Registry::RemoveComponent(const Entity& entity)
	{
		_VerifyEntity(entity);
		m_Buckets[typeid(T).name()].Remove<T>(entity);
	}

	template<typename T>
	inline T& Registry::GetComponent(const EntityID& entity)
	{
		_VerifyEntity(entity);
		return m_Buckets[typeid(T).name()].Get<T>(entity);
	}

	template<typename T, typename Fun>
	inline void Registry::ForEach(Fun fun)
	{
		m_Buckets[typeid(T).name()].ForEach<T>(fun);
	}

	template<typename T>
	inline MemoryAllocator::VariableIterator<T> Registry::View()
	{
		if (!m_Buckets.contains(typeid(T).name()))
			return MemoryAllocator::VariableIterator<T>(nullptr, nullptr);

		return m_Buckets[typeid(T).name()].ViewDense<T>();
	}

}

