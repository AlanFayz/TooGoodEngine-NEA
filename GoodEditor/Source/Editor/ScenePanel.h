#pragma once

#include <Project/Project.h>

#include <unordered_set>

namespace GoodEditor {

	using namespace TooGoodEngine;

	class ScenePanel 
	{
	public:
		static void DrawPanel();

		static EntityID GetSelectedEntity();

	private:
		static void _DrawChildren(Entity& entity,
								  EntityTree& tree, const Ref<Renderer>& sceneRenderer,
								  std::unordered_set<EntityID>& displayed);


		static void _DrawEntity(Entity& entity, EntityTree& tree, const Ref<Renderer>& sceneRenderer);
		static bool _EntityPopup(Entity& entity, const Ref<Renderer>& sceneRenderer, EntityTree& tree);
		static void _DrawSettings(const Ref<Scene>& scene);
	private:
		static int s_IDCount;
		static int s_ColorEditFlags;
		static EntityID s_CurrentEntity;
		static EntityID s_SelectedEntity;
		static bool s_EntityNamePopup;
	};

	
}