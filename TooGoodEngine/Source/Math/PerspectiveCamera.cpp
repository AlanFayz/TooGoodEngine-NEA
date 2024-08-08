#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace TooGoodEngine {

	PerspectiveCamera::PerspectiveCamera(const PerspectiveCameraData& data)
		: m_Projection(), m_View(), m_Position(data.Position), m_Front(data.Front),
		  m_Up(data.Up), m_AspectRatio(data.AspectRatio), m_Near(data.Near), m_Far(data.Far),
		  m_Fov(data.Fov), m_InverseView(), m_InverseProjection()
	{
		UpdateViewProjection();
	}

	void PerspectiveCamera::UpdateViewProjection()
	{
		m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		m_Projection = glm::perspective(m_Fov, m_AspectRatio, m_Near, m_Far);

		m_InverseView = glm::inverse(m_View);
		m_Projection = glm::inverse(m_Projection);
	}

	void PerspectiveCamera::SetData(const PerspectiveCameraData& data)
	{
		m_Position = data.Position;
		m_Front = data.Front;
		m_Up = data.Up;
		m_AspectRatio = data.AspectRatio;
		m_Near = data.Near;
		m_Far = data.Far;
		m_Fov = data.Fov;

		UpdateViewProjection();
	}
	void PerspectiveCamera::OnWindowResize(float newWidth, float newHeight)
	{
		m_AspectRatio = newWidth / newHeight;

		UpdateViewProjection();
	}
}
