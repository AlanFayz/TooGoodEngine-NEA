#pragma once

#include "Core/Files/Json.h"
#include "Renderer/Renderer.h"
#include "ECS/Components/Components.h"

namespace TooGoodEngine {

	class ComponentLoader
	{
	public:
		static TransformComponent		 LoadTransform(const json& jsonComponent);
		static MeshComponent			 LoadMesh(const json& jsonComponent, Renderer& sceneRenderer);
		static ModelComponent            LoadModel(const json& jsonComponent, Renderer& sceneRenderer);
		static MaterialComponent		 LoadMaterial(const json& jsonComponent, Renderer& sceneRenderer);
		static PointLightComponent		 LoadPointLight(const json& jsonComponent);
		static DirectionalLightComponent LoadDirectionalLight(const json& jsonComponent);
	};

}