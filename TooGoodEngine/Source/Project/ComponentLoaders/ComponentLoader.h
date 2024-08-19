#pragma once

#include "Core/Files/Json.h"
#include "Renderer/Renderer.h"
#include "ECS/Components/Components.h"

namespace TooGoodEngine {

	using ModelCache = std::map<uint64_t, ModelInfo>;

	//TODO: should take in the current project as a pointer to its paramters.
	class ComponentLoader
	{
	public:
		static TransformComponent		 LoadTransform(const json& jsonComponent);
		static MeshComponent			 LoadMesh(const json& jsonComponent, Renderer& sceneRenderer);
		static ModelComponent            LoadModel(const json& jsonComponent, Renderer& sceneRenderer);
		static MaterialComponent		 LoadMaterial(const json& jsonComponent, Renderer& sceneRenderer);
		static PointLightComponent		 LoadPointLight(const json& jsonComponent);
		static DirectionalLightComponent LoadDirectionalLight(const json& jsonComponent);

		static void ClearCaches();

	private:
		//using ref instead of stack allocation because was having wierd errors when calling clearcaches.
		//probably a lifetime issue may look into it further
		static Ref<ModelCache> m_ModelCache;
	};

}