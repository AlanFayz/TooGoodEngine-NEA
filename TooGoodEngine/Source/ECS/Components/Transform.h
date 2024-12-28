#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "Core/Base.h"

namespace TooGoodEngine {
	
	struct TransformComponent
	{
		TransformComponent() = default;
		~TransformComponent() = default;
		TransformComponent(
			const glm::vec3& position,
			const glm::vec3& rotation,
			const glm::vec3& scale);

		glm::mat4 GetTransform();

		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
	};
}