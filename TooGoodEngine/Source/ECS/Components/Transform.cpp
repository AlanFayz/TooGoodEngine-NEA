#include "Transform.h"

namespace TooGoodEngine {

	TransformComponent::TransformComponent(
		const glm::vec3& position, 
		const glm::vec3& rotation, 
		const glm::vec3& scale)
		: m_Transform(glm::identity<glm::mat4>()),
			m_Position(position), m_Rotation(rotation),
			m_Scale(scale)
	{
		UpdateTransform();
	}

	void TransformComponent::UpdateTransform()
	{
		glm::mat4 rotationMatrix = glm::rotate(glm::identity<glm::mat4>(), glm::radians(m_Rotation[0]), { 1.0f, 0.0f, 0.0f }) *
			glm::rotate(glm::identity<glm::mat4>(), glm::radians(m_Rotation[1]), { 0.0f, 1.0f, 0.0f }) *
			glm::rotate(glm::identity<glm::mat4>(), glm::radians(m_Rotation[2]), { 0.0f, 0.0f, 1.0f });

		m_Transform = glm::identity<glm::mat4>();
		m_Transform = glm::translate(m_Transform, m_Position) *
			rotationMatrix *
			glm::scale(m_Transform, m_Scale);
	}

	void TransformComponent::Translate(const glm::vec3& offset)
	{
		m_Position += offset;
		UpdateTransform();
	}

	void TransformComponent::Rotate(const glm::vec3& offset)
	{
		m_Rotation += offset;
		UpdateTransform();
	}

	void TransformComponent::Scale(const glm::vec3& offset)
	{
		m_Scale += offset;
		UpdateTransform();
	}
	
}