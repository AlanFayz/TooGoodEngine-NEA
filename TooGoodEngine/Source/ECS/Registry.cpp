#include "Registry.h"

namespace TooGoodEngine{
	Registry::~Registry()
	{
		for (auto& [type, entry] : m_Buckets)
			entry.Deleter(entry.Data);
	}
	void Registry::RemoveEntity(EntityID id)
	{
		m_Entites[id] = Entity("null entity", g_NullEntity);
	}
	Entity Registry::GetEntityByName(const std::string& name)
	{
		for (auto& entity : m_Entites)
		{
			if (entity.GetName() == name)
				return entity;
		}

		return Entity("null entity", g_NullEntity);
	}
	void Registry::_VerifyEntity(const EntityID& entity) const
	{
		TGE_VERIFY(entity < m_Count, "entity is invalid in this registry");
	}

}
