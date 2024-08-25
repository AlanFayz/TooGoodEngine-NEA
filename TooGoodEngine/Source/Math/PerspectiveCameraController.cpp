#include "PerspectiveCameraController.h"


#include <algorithm>
#include "Utils/Input.h"

namespace TooGoodEngine {

	PerspectiveCameraController::PerspectiveCameraController(const Ref<PerspectiveCamera>& camera)
		: m_Camera(camera)
	{
	}
	void PerspectiveCameraController::Update(double delta)
	{
		const bool WKeyDown = Input::IsKeyDown(KeyCode::W);
		const bool SKeyDown = Input::IsKeyDown(KeyCode::S);
		const bool AKeyDown = Input::IsKeyDown(KeyCode::A);
		const bool DKeyDown = Input::IsKeyDown(KeyCode::D);
		const bool QKeyDown = Input::IsKeyDown(KeyCode::Q);
		const bool EKeyDown = Input::IsKeyDown(KeyCode::E);
		const bool XKeyDown = Input::IsKeyPressed(KeyCode::X);

		double CurrentX = 0, CurrentY = 0;
		Input::GetMouseCoordinates(CurrentX, CurrentY);

		float XDifference = ((float)CurrentX - (float)m_LastX) * m_Sensitivity;
		float YDifference = ((float)CurrentY - (float)m_LastY) * m_Sensitivity;

		m_LastX = CurrentX;
		m_LastY = CurrentY;

		if (XKeyDown)
		{
			Input::DisableCursor();
			m_CursorDisabled = true;
		}
		else
		{
			if(m_CursorDisabled)
				Input::EnableCursor();

			m_CursorDisabled = false;
			return;
		}

		glm::vec3 movement(0.0f);

		if (WKeyDown)
			movement -= m_CameraSpeed * m_Camera->m_Up * (float)delta;
		if (SKeyDown)
			movement += m_CameraSpeed * m_Camera->m_Up * (float)delta;

		const glm::vec3 side = glm::normalize(glm::cross(m_Camera->m_Front, m_Camera->m_Up));

		if (AKeyDown)
			movement += m_CameraSpeed * side * (float)delta;
		if (DKeyDown)
			movement -= m_CameraSpeed * side * (float)delta;

		if (QKeyDown)
			movement -= m_CameraSpeed * m_Camera->m_Front * (float)delta;
		if (EKeyDown)
			movement += m_CameraSpeed * m_Camera->m_Front * (float)delta;

		m_Camera->m_Position += movement;

		m_Yaw += XDifference;
		m_Pitch -= YDifference;

		m_Pitch = std::clamp(m_Pitch, -89.0f, 89.0f);

		glm::vec3 Direction(0.0f);
		Direction.x = cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
		Direction.y = sin(glm::radians(m_Pitch));
		Direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

		m_Camera->m_Front = glm::normalize(Direction);

		m_Camera->UpdateViewProjection();
	}
	void PerspectiveCameraController::SetCamera(const Ref<PerspectiveCamera>& newCamera)
	{
		m_Camera = newCamera;
	}
	void PerspectiveCameraController::SetFov(float fov)
	{
		m_Camera->m_Fov = fov;
		m_Camera->UpdateViewProjection();
	}
	void PerspectiveCameraController::SetAspectRatio(float aspectRatio)
	{
		m_Camera->m_AspectRatio = aspectRatio;
		m_Camera->UpdateViewProjection();
	}
	void PerspectiveCameraController::SetNear(float near)
	{
		m_Camera->m_Near = near;
		m_Camera->UpdateViewProjection();
	}
	void PerspectiveCameraController::SetFar(float far)
	{
		m_Camera->m_Far = far;
		m_Camera->UpdateViewProjection();
	}
	void PerspectiveCameraController::UpdatePosition(const glm::vec3& position)
	{
		m_Camera->m_Position = position;
		m_Camera->UpdateViewProjection();
	}
	void PerspectiveCameraController::UpdateFront(const glm::vec3& front)
	{
		m_Camera->m_Front = front;
		m_Camera->UpdateViewProjection();
	}
	void PerspectiveCameraController::UpdateUp(const glm::vec3& up)
	{
		m_Camera->m_Up = up;
		m_Camera->UpdateViewProjection();
	}
}