#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace TooGoodEngine {
	
	class TransformComponent
	{
	public:
		TransformComponent() = default;
		~TransformComponent() = default;
		TransformComponent(
			const glm::vec3& position,
			float rotation,
			const glm::vec3& scale,
			const glm::vec3& rotationAxis);

		void UpdateTransform();
		void Translate(const glm::vec3& offset);
		void Rotate(float offset);
		void UpdateRotationAxis(const glm::vec3& offset);
		void Scale(const glm::vec3& offset);

		inline const glm::mat4& GetTransform() const { return m_Transform; }
		inline glm::vec3& GetPosition() { return m_Position; }
		inline glm::vec3& GetScale()  { return m_Scale; }
		inline glm::vec3& GetRotationAxis()  { return m_RotationAxis; }
		inline float& GetRotation() { return m_Rotation; }

	private:


	private:
		glm::mat4 m_Transform;
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		glm::vec3 m_RotationAxis;
		float m_Rotation;
	};
}