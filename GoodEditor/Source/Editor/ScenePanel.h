#pragma once

#include "Project/Project.h"

#include <unordered_set>

namespace GoodEditor {

	using namespace TooGoodEngine;

	class ScenePanel 
	{
	public:
		static void DrawScenePanel();

	private:
		static void _DrawChildren(const Entity& entity,
								  EntityTree& tree,
								  std::unordered_set<EntityID>& displayed);


		//will display the actual components of the entity
		static void _DrawEntity(const Entity& entity, EntityTree& tree);

		static void _DrawSettings(const Ref<Scene>& scene);
		
		static void _DrawComponent(TransformComponent& component);
		static void _DrawComponent(MeshComponent& component);
		static void _DrawComponent(MaterialComponent& component);

		static bool _DrawMaterialAttribute(const char* name, glm::vec4& attribute, Ref<Image>& image);
		static bool _DrawMaterialRoughness(float& roughness, Ref<Image>& image);


	private:
		static int m_IDCount;
	};

	
}