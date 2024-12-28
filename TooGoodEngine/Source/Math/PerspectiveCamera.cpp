#include "PerspectiveCamera.h"


namespace TooGoodEngine {

	PerspectiveCamera::PerspectiveCamera(const PerspectiveCameraData& data)
		:  m_Position(data.Position), m_Rotation(data.Rotation),
		  m_AspectRatio(data.AspectRatio), m_Near(data.Near), m_Far(data.Far),
		  m_Fov(data.Fov)
	{
	}


	void PerspectiveCamera::SetData(const PerspectiveCameraData& data)
	{
		m_Position = data.Position;
		m_AspectRatio = data.AspectRatio;
		m_Near = data.Near;
		m_Far = data.Far;
		m_Fov = data.Fov;
	}

	void PerspectiveCamera::OnWindowResize(float newWidth, float newHeight)
	{
		m_AspectRatio = newWidth / newHeight;
	}

	const glm::mat4 PerspectiveCamera::GetView()
	{
		glm::vec3 front{};
		front.x = glm::cos(glm::radians(m_Rotation[1])) * glm::cos(glm::radians(m_Rotation[0]));
		front.y = glm::sin(glm::radians(m_Rotation[0]));
		front.z = glm::sin(glm::radians(m_Rotation[1])) * glm::cos(glm::radians(m_Rotation[0]));
		front = glm::normalize(front);

		glm::vec3 up = { 0.0f, 1.0f, 0.0f };

		return glm::lookAt(m_Position, m_Position + front, up);
	}

}
