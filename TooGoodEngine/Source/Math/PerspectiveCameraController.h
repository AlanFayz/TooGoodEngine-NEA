#pragma once

#include "PerspectiveCamera.h"

#include "Core/Base.h"

namespace TooGoodEngine {

	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(const Ref<PerspectiveCamera>& camera);
		~PerspectiveCameraController() = default;

		void Update(double delta);
		void SetCamera(const Ref<PerspectiveCamera>& newCamera);

	private:
		Ref<PerspectiveCamera> m_Camera;

		bool m_ShouldNotUpdate = false;
		float m_CameraSpeed = 1.0f;
		float m_Sensitivity = 1.0f;
		double m_LastX = 0, m_LastY = 0;

		float m_Yaw = 0;
		float m_Pitch = 0;

	};

}