#pragma once

#include "DenseMap.h"
#include "Entity.h"

#include <typeindex>
#include <functional>

namespace TooGoodEngine {

	class Registry
	{
	public:
		Registry() = default;
		~Registry();

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

		virtual void RemoveEntity(EntityID id);

		Entity GetEntityByName(const std::string& name);
		
		template<typename Type>
		inline bool HasComponent(const EntityID& entity) 
		{ 
			if (m_Buckets.contains(typeid(Type)))
			{
				auto bucket = GetBucketAssuredType<Type>();
				return bucket->Contains(entity);
			}

			return false;
		}
	
		template<typename Type>
		void AddComponent(const Entity& entity, const Type& t);

		template<typename Type, typename ...Args>
		void EmplaceComponent(const Entity& entity, Args&&... args);

		template<typename Type>
		void RemoveComponent(const Entity& entity);

		template<typename Type>
		Type& GetComponent(const EntityID& entity);

		template<typename Type, typename Fun>
		void ForEach(Fun fun);

		template<typename Type>
		auto View();

	private:
		void _VerifyEntity(const EntityID& entity) const;

		template<typename Type>
		auto GetBucketAssuredType();

	protected:
		EntityID m_Count = 0;

		std::unordered_map<std::type_index, std::shared_ptr<BaseDenseMap>> m_Buckets;
		std::vector<Entity> m_Entites;
	};

	template<typename Type>
	inline void Registry::AddComponent(const Entity& entity, const Type& t)
	{
		auto bucket = GetBucketAssuredType<Type>();
		bucket->Add(entity, t);
	}

	template<typename Type, typename ...Args>
	inline void Registry::EmplaceComponent(const Entity& entity, Args && ...args)
	{
		auto bucket = GetBucketAssuredType<Type>();
		bucket->Emplace(entity, std::forward<Args>(args)...);
	}

	template<typename Type>
	inline void Registry::RemoveComponent(const Entity& entity)
	{
		auto bucket = GetBucketAssuredType<Type>();
		if (bucket->Contains(entity))
			bucket->Remove(entity);

		TGE_LOG_WARNING("entity ", entity.GetName(), " doesn't contain component ", typeid(Type).name());
	}

	template<typename Type>
	inline Type& Registry::GetComponent(const EntityID& entity)
	{
		auto bucket = GetBucketAssuredType<Type>();

		TGE_VERIFY(bucket->Contains(entity), "doesn't contain component");
		return bucket->Get(entity);
	}

	template<typename Type, typename Fun>
	inline void Registry::ForEach(Fun fun)
	{
		auto bucket = GetBucketAssuredType<Type>();
		bucket->ForEach(fun);
	}

	template<typename Type>
	inline auto Registry::View()
	{
		auto bucket = GetBucketAssuredType<Type>();
		return bucket->ViewDense();
	}

	template<typename Type>
	inline auto Registry::GetBucketAssuredType()
	{
		auto it = m_Buckets.find(typeid(Type));
		if (it != m_Buckets.end()) 
			return std::static_pointer_cast<DenseMap<Type>>(it->second);
		

		m_Buckets[typeid(Type)] = std::make_shared<DenseMap<Type>>();
		return std::static_pointer_cast<DenseMap<Type>>(m_Buckets[typeid(Type)]);
	}

}

