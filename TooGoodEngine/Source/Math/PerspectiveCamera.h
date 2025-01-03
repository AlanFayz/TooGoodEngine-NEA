#pragma once

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace TooGoodEngine {
	
	struct PerspectiveCameraData
	{
		float Fov = 60.0f;
		float AspectRatio = 16.0f / 9.0f;
		float Near = 0.1f;
		float Far = 100.0f;

		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
	};

	class PerspectiveCameraController;

	//another type of camera but it creates a viewing frustum
	//using an angle (field of view) instead of a set of 2D bounds.
	//this is great for 3D games as objects further away become smaller.
	//and vice versa.

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera() = default;
		PerspectiveCamera(const PerspectiveCameraData& data);
		~PerspectiveCamera() = default;

		void SetData(const PerspectiveCameraData& data);
		virtual void OnWindowResize(float newWidth, float newHeight) override;

		glm::mat4 GetTransform();
		glm::mat4 GetOrientation();

		virtual const glm::vec3& GetCameraPosition() { return m_Position; }
		virtual const glm::mat4 GetProjection() { return glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_Near, m_Far);; }
		virtual const glm::mat4 GetView();
		virtual const glm::mat4 GetInverseProjection() { return glm::inverse(GetProjection()); }
		virtual const glm::mat4 GetInverseView() { return glm::inverse(GetView()); }

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;

		float m_Fov;
		float m_AspectRatio;
		float m_Near;
		float m_Far;

		friend class PerspectiveCameraController;
	};

}