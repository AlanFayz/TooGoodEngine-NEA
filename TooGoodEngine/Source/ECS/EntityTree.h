#pragma once

#include "Registry.h"

#include <stack>

namespace TooGoodEngine {

	inline constexpr size_t g_NullNode = std::numeric_limits<size_t>::max();

	struct Node
	{
		std::string_view Name;
		EntityID Entity;
		size_t ParentIndex = g_NullNode;
		std::vector<size_t> Children;
	};

	class EntityTree : public Registry
	{
	public:
		EntityTree() = default;
		~EntityTree() = default;

		Entity Add(const std::string& name);
		Entity Add(const Entity& parent, const std::string& name);

		virtual void RemoveEntity(EntityID id) override;

		const bool ContainsEntity(const Entity& entity);
		Node& GetNode(const Entity& parent);

		/*
			Will move a child entity to a new parent
		*/

		void Move(const Entity& child, const Entity& newParent);

		/*
			Applies function using depth first to elements
			function should take in a name and entity ID
		*/

		template<typename Fun>
		void Depth(const Entity& parent, Fun fun); 


	private:
		size_t _Find(const Entity& entity);
		size_t _FindByName(const std::string& name);

	private:
		std::vector<Node> m_Nodes;
	};

	template<typename Fun>
	inline void EntityTree::Depth(const Entity& parent, Fun fun)
	{
		size_t index = _Find(parent);

		TGE_VERIFY(index != g_NullNode, "parent is not in the tree");

		std::stack<size_t> stack;
		stack.push(index);

		while (!stack.empty())
		{
			Node& currentNode = m_Nodes[stack.top()];
			stack.pop();

			fun(currentNode.Name, currentNode.Entity);

			for (auto child : currentNode.Children)
				stack.push(child);
		}
	}
}

