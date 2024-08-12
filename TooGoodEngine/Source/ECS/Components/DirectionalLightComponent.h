#pragma once

#include <glm/glm.hpp>

namespace TooGoodEngine {
	
	struct DirectionalLightComponent
	{
		DirectionalLightComponent() = default;
		~DirectionalLightComponent() = default;

		glm::vec4 Color;
		glm::vec3 Direction;
		float Intensity;
	};

}