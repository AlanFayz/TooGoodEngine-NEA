#include "Registry.h"


namespace TooGoodEngine {

	static Entity s_NullEntity("null entity", g_NullEntity);

	Registry::~Registry()
	{
		for (auto& [typeIndex, bucket] : m_BucketStorage)
			delete bucket;
	}

	Entity Registry::__CreateEntity(const std::string& name)
	{
		Entity entity = Entity(name, m_EntityStorage.size());
		m_EntityStorage.push_back(entity);
		return entity;
	}

	Entity& Registry::GetEntityByID(EntityID entityID)
	{
		if (entityID < m_EntityStorage.size())
			return m_EntityStorage[entityID];

		return s_NullEntity;
	}

	Entity& Registry::GetEntityByName(const std::string& name)
	{
		for (auto& entity : m_EntityStorage)
		{
			if (entity.GetName() == name)
				return entity;
		}

		return s_NullEntity;
	}

	void Registry::__RemoveEntity(EntityID entityID)
	{
		if (entityID >= m_EntityStorage.size())
			return;

		for (const auto& [typeIndex, bucket] : m_BucketStorage)
			bucket->Remove(entityID);

		m_EntityStorage[entityID] = s_NullEntity;
	}

}