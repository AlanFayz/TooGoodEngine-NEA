#include "Registry.h"

namespace TooGoodEngine{

	void Registry::_VerifyEntity(const EntityID& entity) const
	{
		TGE_VERIFY(entity < m_Count, "entity is invalid in this registry");
	}

}
