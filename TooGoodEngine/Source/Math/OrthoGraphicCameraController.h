#pragma once

#include "Core/Base.h"
#include "OrthographicCamera.h"

namespace TooGoodEngine {

	//TODO: for both perspective and ortho controllers add a way to change settings

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController() = default;
		OrthographicCameraController(const Ref<OrthographicCamera>& camera);
		~OrthographicCameraController() = default;

		void SetCamera(const Ref<OrthographicCamera>& camera);
		void Update(double delta);

	private:
		Ref<OrthographicCamera> m_Camera;

		float m_ZoomFactor = 1.0f;
		float m_CameraSpeed = 10.0f;
	};

}