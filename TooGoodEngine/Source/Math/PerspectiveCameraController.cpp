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
        //query keys
        const bool WKeyDown = Input::IsKeyDown(KeyCode::W);
        const bool SKeyDown = Input::IsKeyDown(KeyCode::S);
        const bool AKeyDown = Input::IsKeyDown(KeyCode::A);
        const bool DKeyDown = Input::IsKeyDown(KeyCode::D);
        const bool QKeyDown = Input::IsKeyDown(KeyCode::Q);
        const bool EKeyDown = Input::IsKeyDown(KeyCode::E);
        const bool XKeyDown = Input::IsKeyPressed(KeyCode::X);

        double CurrentX = 0, CurrentY = 0;
        Input::GetMouseCoordinates(CurrentX, CurrentY);

        float XDifference = (float)(CurrentX - m_LastX) * m_Sensitivity;
        float YDifference = (float)(CurrentY - m_LastY) * m_Sensitivity;

        // update last mouse position
        m_LastX = CurrentX;
        m_LastY = CurrentY;

        // handle cursor toggling
        if (XKeyDown)
        {
            Input::DisableCursor();
            m_CursorDisabled = true;
        }
        else
        {
            if (m_CursorDisabled)
                Input::EnableCursor();

            m_CursorDisabled = false;
            return;
        }

        glm::mat4 orientation = m_Camera->GetOrientation();

        glm::vec3 front = glm::normalize(glm::vec3(orientation * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
        glm::vec3 up    = glm::normalize(glm::vec3(orientation * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)));
        glm::vec3 right = glm::normalize(glm::vec3(orientation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));


        float sign = up.y < 0 ? -1.0f : 1.0f;
        m_Yaw -= XDifference;
        m_Pitch = std::clamp(m_Pitch - YDifference, -89.0f, 89.0f);

        glm::vec3 movement(0.0f);

        if (WKeyDown)
            movement += front * m_CameraSpeed * (float)std::max(delta, 0.001);
     
        if (SKeyDown) 
            movement -= front * m_CameraSpeed * (float)std::max(delta, 0.001);

        if (AKeyDown) 
            movement -= right * m_CameraSpeed * (float)std::max(delta, 0.001);

        if (DKeyDown) 
            movement += right * m_CameraSpeed * (float)std::max(delta, 0.001);

        if (QKeyDown) 
            movement += up * m_CameraSpeed * (float)std::max(delta, 0.001);

        if (EKeyDown) 
            movement -= up * m_CameraSpeed * (float)std::max(delta, 0.001);

        m_Camera->m_Position += movement;
        m_Camera->m_Rotation = { m_Pitch, m_Yaw, 0.0f };
    }

	void PerspectiveCameraController::SetCamera(const Ref<PerspectiveCamera>& newCamera)
	{
		m_Camera = newCamera;
	}
	void PerspectiveCameraController::SetFov(float fov)
	{
		m_Camera->m_Fov = fov;
	}
	void PerspectiveCameraController::SetAspectRatio(float aspectRatio)
	{
		m_Camera->m_AspectRatio = aspectRatio;
	}
	void PerspectiveCameraController::SetNear(float near)
	{
		m_Camera->m_Near = near;
	}
	void PerspectiveCameraController::SetFar(float far)
	{
		m_Camera->m_Far = far;
	}
	void PerspectiveCameraController::UpdatePosition(const glm::vec3& position)
	{
		m_Camera->m_Position = position;
	}
	void PerspectiveCameraController::UpdateRotation(const glm::vec3& rotation)
	{
		m_Camera->m_Rotation = rotation;
	}
	void PerspectiveCameraController::SetCameraSpeed(float newSpeed)
	{
		m_CameraSpeed = newSpeed;
	}
	void PerspectiveCameraController::SetCameraSensitivity(float newSensitivity)
	{
		m_Sensitivity = newSensitivity;
	}
}