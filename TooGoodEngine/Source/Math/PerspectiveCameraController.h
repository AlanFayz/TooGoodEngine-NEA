#pragma once

#include "PerspectiveCamera.h"

#include "Core/Base.h"

namespace TooGoodEngine {

	//simply controls a perspective camera.
	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(const Ref<PerspectiveCamera>& camera);
		~PerspectiveCameraController() = default;

		//Update is the default camera movement but more can be programmed using functions below.
		void Update(double delta);
		void SetCamera(const Ref<PerspectiveCamera>& newCamera);

		void SetFov(float fov);
		void SetAspectRatio(float aspectRatio);
		void SetNear(float near);
		void SetFar(float far);

		void UpdatePosition(const glm::vec3& position);
		void UpdateFront(const glm::vec3& front);
		void UpdateUp(const glm::vec3& up);

		void SetCameraSpeed(float newSpeed);
		void SetCameraSensitivity(float newSensitivity);

	private:
		Ref<PerspectiveCamera> m_Camera;

		bool m_CursorDisabled = false;
		float m_CameraSpeed = 100.0f;
		float m_Sensitivity = 1.0f;
		double m_LastX = 0, m_LastY = 0;

		float m_Yaw = 0;
		float m_Pitch = 0;

	};

}