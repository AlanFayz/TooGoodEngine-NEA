#include "EntityTree.h"

#include "Core/Base.h"

namespace TooGoodEngine {

	Entity EntityTree::Add(const std::string& name)
	{
		Entity entity = __CreateEntity(name);

		//all nodes are stored in one vector.
		Node node{};
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

		Entity entity = __CreateEntity(name);

		Node child{};
		child.Entity = entity;
		child.ParentIndex = index;

		m_Nodes.push_back(child);

		Node& parentNode = m_Nodes[index];
		parentNode.Children.push_back(m_Nodes.size() - 1);


		return entity;
	}

	void EntityTree::RemoveEntity(EntityID id)
	{
		//invalid id.
		if (id >= GetCount())
			return;

		//find the node where the entity is.
		size_t nodeIndex = _Find(GetEntityByID(id));
		if (nodeIndex == g_NullNode)
			return;

		//remove all its children recursively.
		for (auto& child : m_Nodes[nodeIndex].Children)
			RemoveEntity(child);

		Entity entity = GetEntityByID(id);
		nodeIndex = _Find(entity); //nodeIndex may have changed from previous erases

		//remove the entity from the node and from the registry.
		//this also automatically frees any componenents associated with the entity.
		m_Nodes.erase(m_Nodes.begin() + nodeIndex);
		__RemoveEntity(entity);
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
		size_t childIndex = _Find(child);
		size_t parentIndex = _Find(newParent);

		TGE_VERIFY(childIndex != g_NullNode && parentIndex != g_NullNode, "child or parent was not found");

		auto& childNode = m_Nodes[childIndex];
		auto& parentNode = m_Nodes[parentIndex];

		//if there is no parent index to the child then that means its a root. We can simply append 
		//its index to the parent and return.
		if (childNode.ParentIndex == g_NullNode)
		{
			parentNode.Children.push_back(childIndex);
			childNode.ParentIndex = parentIndex;
			return;
		}

		//if it does have an old parent then we need to adjust that parents children
		//by removing this child.
		size_t oldParentIndex = m_Nodes[childIndex].ParentIndex;
		auto& oldParentNode = m_Nodes[oldParentIndex];

		auto it = std::find(oldParentNode.Children.begin(), oldParentNode.Children.end(), childIndex);

		if (it != oldParentNode.Children.end())
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
	size_t EntityTree::_FindByName(const std::string& name)
	{
		auto it = std::find_if(m_Nodes.begin(), m_Nodes.end(),
			[&](const Node& node)
			{
				return GetEntityByID(node.Entity).GetName() == name;
			});

		if (it != m_Nodes.end())
			return (size_t)std::distance(m_Nodes.begin(), it);

		return g_NullNode;
	}
}