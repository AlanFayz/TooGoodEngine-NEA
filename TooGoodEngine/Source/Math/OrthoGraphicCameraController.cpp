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

		glm::vec3 up   = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 side = glm::vec3(1.0f, 0.0f, 0.0f);

		if (keyWPressed)
			movement += up * m_CameraSpeed * (float)delta;
		if (keySPressed)
			movement -= up * m_CameraSpeed * (float)delta;
		if (keyAPressed)
			movement -= side * m_CameraSpeed * (float)delta;
		if (keyDPressed)
			movement += side * m_CameraSpeed * (float)delta;

		m_Camera->m_Position += movement;
	}

	void OrthographicCameraController::UpdatePosition(const glm::vec3& position)
	{
		m_Camera->m_Position = position;
	}
	void OrthographicCameraController::UpdateUp(const glm::vec3& up)
	{
	}
	void OrthographicCameraController::UpdateFront(const glm::vec3& front)
	{
	}
	void OrthographicCameraController::SetLeft(float left)
	{
		m_Camera->m_Left = left;
	}
	void OrthographicCameraController::SetRight(float right)
	{
		m_Camera->m_Right = right;
	}
	void OrthographicCameraController::SetTop(float top)
	{
		m_Camera->m_Top = top;
	}
	void OrthographicCameraController::SetBottom(float bottom)
	{
		m_Camera->m_Bottom = bottom;
	}

	void OrthographicCameraController::SetCameraSpeed(float newSpeed)
	{
		m_CameraSpeed = newSpeed;
	}

}
