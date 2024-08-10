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

		glm::vec3 Movement(0.0f);

		if (WKeyDown)
			Movement -= m_CameraSpeed * m_Camera->m_Up * (float)delta;
		if (SKeyDown)
			Movement += m_CameraSpeed * m_Camera->m_Up * (float)delta;

		const glm::vec3 Side = glm::normalize(glm::cross(m_Camera->m_Front, m_Camera->m_Up));

		if (AKeyDown)
			Movement += m_CameraSpeed * Side * (float)delta;
		if (DKeyDown)
			Movement -= m_CameraSpeed * Side * (float)delta;

		if (QKeyDown)
			Movement -= m_CameraSpeed * m_Camera->m_Front * (float)delta;
		if (EKeyDown)
			Movement += m_CameraSpeed * m_Camera->m_Front * (float)delta;

		m_Camera->m_Position += Movement;

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
}