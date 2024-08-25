#pragma once

#include "SparseSet.h"

#include <typeindex>
#include <unordered_map>
#include <memory>

namespace TooGoodEngine {

	class Registry
	{
	public:
		using EntityStorage = std::vector<Entity>;
		using BucketStorage = std::unordered_map<std::type_index, BaseSparseSet*>;

	public:
		Registry() = default;
		~Registry();

		inline const size_t GetCount() const { return m_EntityStorage.size(); }

		Entity& GetEntityByID(EntityID entityID);
		Entity& GetEntityByName(const std::string& name);

		template<typename Type>
		inline const bool HasComponent(const EntityID& entity);

		template<typename Type>
		void AddComponent(EntityID entityID, const Type& type);

		template<typename Type, typename ...Args>
		void EmplaceComponent(EntityID entityID, Args&&... args);

		template<typename Type>
		void RemoveComponent(EntityID entityID);

		//assumes user has already done the check to see if it exists
		template<typename Type>
		Type& GetComponent(EntityID entityID);

		template<typename Type, typename Fun>
		void ForEach(Fun fun);

		template<typename Type>
		const auto View();

	protected:
		//to be used by inheriting system

		void    __RemoveEntity(EntityID entityID);
		Entity  __CreateEntity(const std::string& name);

	private:
		template<typename Type>
		SparseSet<Type>* _GetBucketAssuredType();


	private:
		EntityStorage m_EntityStorage;
		BucketStorage m_BucketStorage;
	};

	template<typename Type>
	inline const bool Registry::HasComponent(const EntityID& entity)
	{
		if (m_BucketStorage.contains(typeid(Type)))
		{
			auto bucket = _GetBucketAssuredType<Type>();
			return bucket->Contains(entity);
		}

		return false;
	}

	template<typename Type>
	inline void Registry::AddComponent(EntityID entityID, const Type& type)
	{
		auto bucket = _GetBucketAssuredType<Type>();
		bucket->Insert(entityID, type);
	}

	template<typename Type, typename ...Args>
	inline void Registry::EmplaceComponent(EntityID entityID, Args && ...args)
	{
		auto bucket = _GetBucketAssuredType<Type>();
		bucket->Emplace(entityID, std::forward<Args>(args)...);
	}

	template<typename Type>
	inline void Registry::RemoveComponent(EntityID entityID)
	{
		auto bucket = _GetBucketAssuredType<Type>();
		bucket->Remove(entityID);
	}

	template<typename Type>
	inline Type& Registry::GetComponent(EntityID entityID)
	{
		auto bucket = _GetBucketAssuredType<Type>();

		TGE_VERIFY(bucket->Contains(entityID), "doesn't contain component");
		return bucket->Get(entityID);
	}

	template<typename Type, typename Fun>
	inline void Registry::ForEach(Fun fun)
	{
		auto bucket = _GetBucketAssuredType<Type>();
		for (auto& [component, entityID] : *bucket)
			fun(component, entityID);
	}

	template<typename Type>
	inline const auto Registry::View()
	{
		auto bucket = _GetBucketAssuredType<Type>();
		return bucket->GetDense();
	}

	template<typename Type>
	inline SparseSet<Type>* Registry::_GetBucketAssuredType()
	{
		const std::type_index typeID = typeid(Type);

		//trying to find type if its in bucket storage
		auto it = m_BucketStorage.find(typeID);
		if (it != m_BucketStorage.end())
			return (SparseSet<Type>*)(it->second);

		//using normal pointers here because the overhead with shared_ptr
		//was too much (40% on the profiler)
		//so just using new/delete keys normally. FIGHT ME. unique_ptrs ownership semantics
		//are too annoying to deal with as well and don't look clean.

		SparseSet<Type>* bucket = new SparseSet<Type>();
		m_BucketStorage[typeID] = bucket;
		return bucket;
	}

}