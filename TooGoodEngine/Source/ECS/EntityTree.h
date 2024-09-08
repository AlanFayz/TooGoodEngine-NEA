#pragma once

#include "Registry.h"

#include <stack>

namespace TooGoodEngine {

	inline constexpr size_t g_NullNode = std::numeric_limits<size_t>::max();

	struct Node
	{
		EntityID Entity;
		size_t ParentIndex = g_NullNode;
		std::vector<size_t> Children;
	};

	//this is a type of registry which organises entities in a hierarchical relationship
	//where entities can inherit each other.
	class EntityTree : public Registry
	{
	public:
		EntityTree() = default;
		~EntityTree() = default;

		Entity Add(const std::string& name);
		Entity Add(const Entity& parent, const std::string& name);

		void RemoveEntity(EntityID id);

		const bool ContainsEntity(const Entity& entity);
		Node& GetNode(const Entity& parent);

		void Move(const Entity& child, const Entity& newParent);


	private:
		size_t _Find(const Entity& entity);
		size_t _FindByName(const std::string& name);

	private:
		std::vector<Node> m_Nodes;
	};

}

