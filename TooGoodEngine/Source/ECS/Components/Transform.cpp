#include "Transform.h"

namespace TooGoodEngine {

	TransformComponent::TransformComponent(
		const glm::vec3& position, 
		float rotation, 
		const glm::vec3& scale, 
		const glm::vec3& rotationAxis)
		: m_Transform(glm::identity<glm::mat4>()),
			m_Position(position), m_Rotation(rotation),
			m_Scale(scale), m_RotationAxis(rotationAxis)
	{
		UpdateTransform();
	}

	void TransformComponent::UpdateTransform()
	{
		m_RotationAxis = glm::clamp(m_RotationAxis, { FLT_MIN, FLT_MIN, FLT_MIN }, { 1.0f, 1.0f, 1.0f });

		m_Transform = glm::identity<glm::mat4>();
		m_Transform = glm::translate(m_Transform, m_Position) *
			glm::rotate(m_Transform, m_Rotation, m_RotationAxis) *
			glm::scale(m_Transform, m_Scale);
	}

	void TransformComponent::Translate(const glm::vec3& offset)
	{
		m_Position += offset;
		UpdateTransform();
	}

	void TransformComponent::Rotate(float offset)
	{
		m_Rotation += offset;
		UpdateTransform();
	}

	void TransformComponent::UpdateRotationAxis(const glm::vec3& offset)
	{
		m_RotationAxis += offset;
		m_RotationAxis = glm::clamp(
			m_RotationAxis, { FLT_MIN, FLT_MIN, FLT_MIN }, { 1.0f, 1.0f, 1.0f });

		UpdateTransform();
	}

	void TransformComponent::Scale(const glm::vec3& offset)
	{
		m_Scale += offset;
		UpdateTransform();
	}
	
}