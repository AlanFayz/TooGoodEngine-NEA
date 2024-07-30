#pragma once

#include <string_view>

namespace TooGoodEngine {
	
	using EntityID = size_t;

	class Entity
	{
	public:
		Entity() = default;
		~Entity() = default;

		inline const std::string_view GetName() const { return m_Name; }
		inline const EntityID GetID() const { return m_ID; }

		inline operator size_t() const { return m_ID; }
		inline operator std::string_view() const { return m_Name; }
		inline operator const char* () const { return m_Name.data(); }

	private:
		Entity(std::string_view name, EntityID id);

	private:
		std::string_view m_Name;
		EntityID m_ID;

		friend class Registry;
	};
}

