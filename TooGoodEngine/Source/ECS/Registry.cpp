#include "Registry.h"

namespace TooGoodEngine{
	Registry::~Registry()
	{
		for (auto& [type, entry] : m_Buckets)
			entry.Deleter(entry.Data);
	}
	void Registry::_VerifyEntity(const EntityID& entity) const
	{
		TGE_VERIFY(entity < m_Count, "entity is invalid in this registry");
	}

}
