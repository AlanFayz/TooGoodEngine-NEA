#include "EntityTree.h"

namespace TooGoodEngine {

	Entity EntityTree::Add(const std::string& name)
	{
		Entity entity = CreateEntity(name);

		Node node{};
		node.Name = entity;
		node.Entity = entity;
		node.ParentIndex = g_NullNode;

		m_Nodes.push_back(node);

		return entity;
	}

	Entity EntityTree::Add(const Entity& parent, const std::string& name)
	{
		size_t index = _Find(parent);

		if (index == g_NullNode)
		{
			TGE_LOG_WARNING("parent not found adding entity without parent");
			return Add(name);
		}

		Entity entity = CreateEntity(name);
	
		Node child{};
		child.Name = entity;
		child.Entity = entity;
		child.ParentIndex = index;
		
		m_Nodes.push_back(child);

		Node& parentNode = m_Nodes[index];
		parentNode.Children.push_back(m_Nodes.size() - 1);

		return entity;
	}

	const bool EntityTree::ContainsEntity(const Entity& entity)
	{
		return _Find(entity) != g_NullNode;
	}

	Node& EntityTree::GetNode(const Entity& parent)
	{
		size_t index = _Find(parent);

		TGE_VERIFY(index != g_NullNode, "parent not found");

		return m_Nodes[index];
	}

	void EntityTree::Move(const Entity& child, const Entity& newParent)
	{
		size_t childIndex  = _Find(child);
		size_t parentIndex = _Find(newParent);

		TGE_VERIFY(childIndex != g_NullNode && parentIndex != g_NullNode, "child or parent was not found");

		auto& childNode  = m_Nodes[childIndex];
		auto& parentNode = m_Nodes[parentIndex];

		if (childNode.ParentIndex == g_NullNode)
		{
			parentNode.Children.push_back(childIndex);
			childNode.ParentIndex = parentIndex;
			return;
		}

		size_t oldParentIndex = m_Nodes[childIndex].ParentIndex;
		auto& oldParentNode = m_Nodes[oldParentIndex];

		auto it = std::find(oldParentNode.Children.begin(), oldParentNode.Children.end(), childIndex);

		if(it != oldParentNode.Children.end())
			oldParentNode.Children.erase(it);

		parentNode.Children.push_back(childIndex);
		childNode.ParentIndex = parentIndex;
	}

	size_t EntityTree::_Find(const Entity& entity)
	{
		auto it = std::find_if(m_Nodes.begin(), m_Nodes.end(), 
			[&](const Node& node) 
			{
				return node.Entity == entity.GetID();
			});

		if (it != m_Nodes.end())
			return (size_t)std::distance(m_Nodes.begin(), it);

		return g_NullNode;
	}
}