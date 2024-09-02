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
		static constexpr glm::mat4 s_Identity = glm::identity<glm::mat4>();

		glm::mat4 rotationMatrix = glm::rotate(s_Identity, glm::radians(Rotation[0]), { 1.0f, 0.0f, 0.0f }) *
			glm::rotate(s_Identity, glm::radians(Rotation[1]), { 0.0f, 1.0f, 0.0f }) *
			glm::rotate(s_Identity, glm::radians(Rotation[2]), { 0.0f, 0.0f, 1.0f });

		return glm::translate(s_Identity, Position) * rotationMatrix * glm::scale(s_Identity, Scale);
	}
	
}