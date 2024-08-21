#pragma once

#include "Core/Base.h"
#include "OrthographicCamera.h"

namespace TooGoodEngine {


	class OrthographicCameraController
	{
	public:
		OrthographicCameraController() = default;
		OrthographicCameraController(const Ref<OrthographicCamera>& camera);
		~OrthographicCameraController() = default;

		void SetCamera(const Ref<OrthographicCamera>& camera);
		void Update(double delta); //for default camera controls

		void TranslatePosition(const glm::vec3& translation);
		void TranslateUp(const glm::vec3& translation);
		void TranslateFront(const glm::vec3& translation);

		void SetLeft(float left);
		void SetRight(float right);
		void SetTop(float top);
		void SetBottom(float bottom);
		void SetZoomFactor(float zoom);

	private:
		Ref<OrthographicCamera> m_Camera;

		float m_CameraSpeed = 10.0f;
	};

}