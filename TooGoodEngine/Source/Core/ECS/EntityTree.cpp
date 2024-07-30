#include "EntityTree.h"

namespace TooGoodEngine {

	Entity EntityTree::Add(std::string_view name)
	{
		Entity entity = CreateEntity(name);

		Node node{};
		node.Name = entity;
		node.Entity = entity;
		node.ParentIndex = g_NullNode;

		m_Nodes.push_back(node);

		return entity;
	}

	Entity EntityTree::Add(const Entity& parent, std::string_view name)
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
		parentNode.Children.insert(m_Nodes.size() - 1);

		return entity;
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