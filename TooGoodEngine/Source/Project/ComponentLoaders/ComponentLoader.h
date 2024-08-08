#pragma once

#include "Core/Files/Json.h"
#include "Renderer/Renderer.h"
#include "ECS/Components/Components.h"

namespace TooGoodEngine {

	class ComponentLoader
	{
	public:
		static TransformComponent LoadTransform(const json& jsonComponent);
		static MeshComponent LoadMesh(const json& jsonComponent, Renderer& sceneRenderer);
	};

}