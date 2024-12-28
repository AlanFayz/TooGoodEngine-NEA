#include "Transform.h"

namespace TooGoodEngine {

	TransformComponent::TransformComponent(
		const glm::vec3& position, 
		const glm::vec3& rotation, 
		const glm::vec3& scale)
			: Position(position), Rotation(rotation),
			  Scale(scale)
	{
	}

	glm::mat4 TransformComponent::GetTransform()
	{
		constexpr glm::mat4 identity = glm::identity<glm::mat4>();

		glm::mat4 rotationMatrix = glm::rotate(identity, glm::radians(Rotation[0]), { 1.0f, 0.0f, 0.0f }) *
			glm::rotate(identity, glm::radians(Rotation[1]), { 0.0f, 1.0f, 0.0f }) *
			glm::rotate(identity, glm::radians(Rotation[2]), { 0.0f, 0.0f, 1.0f });

		return glm::translate(identity, Position) * rotationMatrix * glm::scale(identity, Scale);
	}
	
}