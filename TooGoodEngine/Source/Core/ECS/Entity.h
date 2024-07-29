#pragma once

#include <string_view>

namespace TooGoodEngine {
	
	using EntityID = size_t;

	class Entity
	{
	public:
		Entity() = default;
		~Entity() = default;

		Entity(std::string_view name, EntityID id);

		inline const std::string_view GetName() const { return m_Name; }
		inline const EntityID GetID() const { return m_ID; }

	private:
		std::string_view m_Name;
		EntityID m_ID;
	};
}

