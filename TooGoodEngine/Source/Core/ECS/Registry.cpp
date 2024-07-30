#include "Registry.h"

namespace TooGoodEngine{

	void Registry::_VerifyEntity(const Entity& entity) const
	{
		TGE_VERIFY(entity.GetID() < m_Count, "entity is invalid in this registry");
	}

}
