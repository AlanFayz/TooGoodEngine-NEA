#include "Entity.h"

namespace TooGoodEngine {

	Entity::Entity(std::string_view name, EntityID id)
		: m_Name(name), m_ID(id)
	{
	}

}