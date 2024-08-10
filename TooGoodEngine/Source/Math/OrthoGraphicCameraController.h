#pragma once

#include "Core/Base.h"
#include "OrthoGraphicCamera.h"

namespace TooGoodEngine {

	//TODO: for both perspective and ortho controllers add a way to change settings

	class OrthoGraphicCameraController
	{
	public:
		OrthoGraphicCameraController() = default;
		OrthoGraphicCameraController(const Ref<OrthoGraphicCamera>& camera);
		~OrthoGraphicCameraController() = default;

		void SetCamera(const Ref<OrthoGraphicCamera>& camera);
		void Update(double delta);

	private:
		Ref<OrthoGraphicCamera> m_Camera;

		float m_ZoomFactor = 1.0f;
		float m_CameraSpeed = 10.0f;
	};

}