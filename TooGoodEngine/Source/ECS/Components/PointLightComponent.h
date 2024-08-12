#pragma once

#include <glm/glm.hpp>

namespace TooGoodEngine {

	struct PointLightComponent
	{
		PointLightComponent() = default;
		~PointLightComponent() = default;

		glm::vec4 Color;
		glm::vec3 Position;
		float Radius;
		float Intensity;
	};

}