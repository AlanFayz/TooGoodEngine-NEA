#pragma once

#include "Renderer/RenderInfo.h"

namespace TooGoodEngine {

	struct MaterialComponent
	{
		MaterialComponent() = default;
		~MaterialComponent() = default;

		MaterialID ID;
		//keeping a copy here so we can serialize it 
		Material Material; 
	};

}