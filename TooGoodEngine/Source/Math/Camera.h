#pragma once

#include <glm/glm.hpp>

namespace TooGoodEngine {

	class Camera
	{
	public:
		virtual const glm::vec3& GetCameraPosition() = 0;
		virtual const glm::mat4& GetProjection() = 0;
		virtual const glm::mat4& GetView() = 0;
		virtual const glm::mat4& GetInverseProjection() = 0;
		virtual const glm::mat4& GetInverseView() = 0;
	};

}