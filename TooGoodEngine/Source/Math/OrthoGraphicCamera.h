#pragma once

#include "Camera.h"

namespace TooGoodEngine {

	struct OrthographicCameraData
	{
		glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

		float Left = 0.0f;
		float Right = 0.0f;
		float Top = 0.0f;
		float Bottom = 0.0f;
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

		void UpdateViewProjection();

		virtual const glm::vec3& GetCameraPosition() { return m_Position; }
		virtual const glm::mat4& GetProjection() { return m_Projection; }
		virtual const glm::mat4& GetView() { return m_View; }
		virtual const glm::mat4& GetInverseProjection() { return m_InverseProjection; }
		virtual const glm::mat4& GetInverseView() { return m_InverseView; }

	private:
		glm::mat4 m_View;
		glm::mat4 m_Projection;

		glm::mat4 m_InverseView;
		glm::mat4 m_InverseProjection;

		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_Up;

		float m_Top, m_Bottom, m_Left, m_Right;

		friend class OrthographicCameraController;
	};

}