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

		void UpdatePosition(const glm::vec3& translation);
		void UpdateUp(const glm::vec3& translation);
		void UpdateFront(const glm::vec3& translation);

		void SetLeft(float left);
		void SetRight(float right);
		void SetTop(float top);
		void SetBottom(float bottom);

	private:
		Ref<OrthographicCamera> m_Camera;

		float m_CameraSpeed = 10.0f;
	};

}