#pragma once

#include "Assets/Model.h"
#include "Renderer/Renderer.h"

namespace TooGoodEngine {

	struct ModelComponent
	{
		ModelComponent() = default;
		~ModelComponent() = default;
		ModelComponent(UUID id) : ModelAssetId(id) {};


		UUID ModelAssetId;
		ModelInfo Info;
	};

}