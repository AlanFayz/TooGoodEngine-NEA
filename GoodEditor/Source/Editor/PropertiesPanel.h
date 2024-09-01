#pragma once

#include "ScenePanel.h"

namespace GoodEditor {

	class PropertiesPanel
	{
	public:
		static void DrawPanel();

	private:
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
		inline static int s_IDCount = 100000;
		static int s_ColorEditFlags;

	};

}