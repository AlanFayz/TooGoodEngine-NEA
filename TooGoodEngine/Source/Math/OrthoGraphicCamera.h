#pragma once


#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace TooGoodEngine {

	struct OrthographicCameraData
	{
		glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f, 0.0f, 0.0f);

		float Left   = -1.0f;
		float Right  = 1.0f;
		float Top    = 1.0f;
		float Bottom = -1.0f;
	};

	class OrthographicCameraController;

	//a type of camera which creates a viewing box essentially.
	//there is no concept of scale when objects are further away
	//this is used for 2D games mainly but can be used for 3D if there is some
	//art style
	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera() = default;
		OrthographicCamera(const OrthographicCameraData& data);
		~OrthographicCamera() = default;

		void SetData(const OrthographicCameraData& data);
		virtual void OnWindowResize(float newWidth, float newHeight) override;

		virtual const glm::vec3& GetCameraPosition() { return m_Position; }

		glm::mat4 GetTransform();

		virtual const glm::mat4 GetProjection() { return glm::ortho(m_Left, m_Right, m_Bottom, m_Top, -1.0f, 1.0f);  }
		virtual const glm::mat4 GetView() { return glm::inverse(GetTransform()); }
		virtual const glm::mat4 GetInverseProjection() { return glm::inverse(GetProjection()); }
		virtual const glm::mat4 GetInverseView() { return GetTransform(); }

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;

		float m_Top, m_Bottom, m_Left, m_Right;

		friend class OrthographicCameraController;
	};

}