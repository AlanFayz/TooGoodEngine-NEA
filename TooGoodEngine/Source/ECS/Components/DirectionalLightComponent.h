#pragma once

#include <glm/glm.hpp>

namespace TooGoodEngine {
	
	struct DirectionalLightComponent
	{
		DirectionalLightComponent() = default;
		~DirectionalLightComponent() = default;

		glm::vec4 Color = { 0.0f, 0.0f, 0.0f, 0.0f };
		glm::vec3 Direction = { 1.0f, 0.0f, 0.0f };
		float Intensity = 1.0f;
	};

}