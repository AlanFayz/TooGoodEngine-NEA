#pragma once

#include <string>
#include <limits>

namespace TooGoodEngine {
	
	using EntityID = size_t;

	inline constexpr EntityID g_NullEntity = std::numeric_limits<size_t>::max();

	class Entity
	{
	public:
		Entity(const std::string& name, EntityID id);
		Entity() = default;
		~Entity() = default;

		inline const std::string& GetName() const { return m_Name; }
		inline const EntityID GetID() const { return m_ID; }

		inline operator size_t() const { return m_ID; }
		inline operator std::string() const { return m_Name; }
		inline operator const char* () const { return m_Name.data(); }
		inline operator bool() const { return m_ID != g_NullEntity; }

	private:
		std::string m_Name;
		EntityID m_ID;

		friend class Registry;
	};
}

