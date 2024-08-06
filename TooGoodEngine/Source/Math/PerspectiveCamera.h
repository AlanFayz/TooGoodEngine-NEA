#pragma once

#include "Camera.h"

namespace TooGoodEngine {
	
	struct PerspectiveCameraData
	{
		float Fov = 60.0f;
		float AspectRatio = 16.0f / 9.0f;
		float Near = 0.1f;
		float Far = 100.0f;

		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Front = { 0.0f, 0.0f, 1.0f };
		glm::vec3 Up = { 0.0f, 1.0f, 0.0f };
	};

	class PerspectiveCameraController;

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera() = default;
		PerspectiveCamera(const PerspectiveCameraData& data);
		~PerspectiveCamera() = default;

		void UpdateViewProjection();
		void SetData(const PerspectiveCameraData& data);
		virtual void OnWindowResize(float newWidth, float newHeight) override;

		virtual const glm::vec3& GetCameraPosition() { return m_Position; }
		virtual const glm::mat4& GetProjection() { return m_Projection; }
		virtual const glm::mat4& GetView() { return m_View; }
		virtual const glm::mat4& GetInverseProjection() { return m_InverseProjection; }
		virtual const glm::mat4& GetInverseView() { return m_InverseView; }

	private:
		glm::mat4 m_Projection;
		glm::mat4 m_View;

		glm::mat4 m_InverseProjection;
		glm::mat4 m_InverseView;

		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_Up;

		float m_Fov;
		float m_AspectRatio;
		float m_Near;
		float m_Far;

		friend class PerspectiveCameraController;
	};

}