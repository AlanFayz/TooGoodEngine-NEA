#pragma once

#include <Project/Project.h>

#include <unordered_set>

namespace GoodEditor {

	using namespace TooGoodEngine;

	class ScenePanel 
	{
	public:
		static void DrawPanel();

	private:
		static void _DrawChildren(Entity& entity,
								  EntityTree& tree, const Ref<Renderer>& sceneRenderer,
								  std::unordered_set<EntityID>& displayed);


		static void _DrawEntity(Entity& entity, EntityTree& tree, const Ref<Renderer>& sceneRenderer);
		static bool _EntityPopup(Entity& entity, const Ref<Renderer>& sceneRenderer, EntityTree& tree);
		static void _DrawSettings(const Ref<Scene>& scene);
		
		static void _DrawComponent(TransformComponent& component);
		static void _DrawComponent(MeshComponent& component);
		static void _DrawComponent(MaterialComponent& component);
		static void _DrawComponent(PointLightComponent& component);
		static void _DrawComponent(DirectionalLightComponent& component);
		static void _DrawComponent(ModelComponent& component);
		static void _DrawComponent(ScriptComponent& component);
		static void _DrawComponent(PerspectiveCameraComponent& component);
		static void _DrawComponent(OrthographicCameraComponent& component);

		static bool _DrawMaterialAttribute(const char* name, glm::vec4& attribute, Ref<Image>& image);
		static bool _DrawMaterialSingle(const char* name, float& value, Ref<Image>& image);

		static bool _DeleteComponentPopup(const char* name);
	private:
		static int m_IDCount;
		static int m_ColorEditFlags;
		static EntityID m_CurrentEntity;
		static bool m_EntityNamePopup;
	};

	
}