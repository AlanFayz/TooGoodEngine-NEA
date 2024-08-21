#include "OrthographicCameraController.h"

#include "Utils/Input.h"

namespace TooGoodEngine {
	OrthographicCameraController::OrthographicCameraController(const Ref<OrthographicCamera>& camera)
		: m_Camera(camera)
	{
	}
	void OrthographicCameraController::SetCamera(const Ref<OrthographicCamera>& camera)
	{
		m_Camera = camera;
	}
	void OrthographicCameraController::Update(double delta)
	{
		const bool keyWPressed = Input::IsKeyDown(KeyCode::W);
		const bool keySPressed = Input::IsKeyDown(KeyCode::S);
		const bool keyAPressed = Input::IsKeyDown(KeyCode::A);
		const bool keyDPressed = Input::IsKeyDown(KeyCode::D);

		glm::vec3 movement(0.0f);

		glm::vec3 side = glm::normalize(glm::cross(m_Camera->m_Front, m_Camera->m_Up));

		if (keyWPressed)
			movement += m_CameraSpeed * m_Camera->m_Up * (float)delta;
		if (keySPressed)
			movement -= m_CameraSpeed * m_Camera->m_Up * (float)delta;
		if (keyAPressed)
			movement -= side * m_CameraSpeed * (float)delta;
		if (keyDPressed)
			movement += side * m_CameraSpeed * (float)delta;

		m_Camera->m_Position += movement;

		m_Camera->UpdateViewProjection();
	}

	void OrthographicCameraController::TranslatePosition(const glm::vec3& translation)
	{
	}
	void OrthographicCameraController::TranslateUp(const glm::vec3& translation)
	{
	}
	void OrthographicCameraController::TranslateFront(const glm::vec3& translation)
	{
	}
	void OrthographicCameraController::SetLeft(float left)
	{
	}
	void OrthographicCameraController::SetRight(float right)
	{
	}
	void OrthographicCameraController::SetTop(float top)
	{
	}
	void OrthographicCameraController::SetBottom(float bottom)
	{
	}
	void OrthographicCameraController::SetZoomFactor(float zoom)
	{
	}
}
