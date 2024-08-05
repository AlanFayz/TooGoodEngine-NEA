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


		void Translate(const glm::vec3& offset);
		void Rotate(float offset);
		void UpdateRotationAxis(const glm::vec3& offset);
		void Scale(const glm::vec3& offset);

		inline const glm::mat4& GetTransform() const { return m_Transform; }
		inline const glm::vec3& GetPosition() const { return m_Position; }
		inline const glm::vec3& GetScale() const { return m_Scale; }
		inline const glm::vec3& GetRotationAxis() const { return m_RotationAxis; }
		inline float GetRotation() const { return m_Rotation; }

	private:
		void _UpdateTransform();


	private:
		glm::mat4 m_Transform;
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		glm::vec3 m_RotationAxis;
		float m_Rotation;
	};
}