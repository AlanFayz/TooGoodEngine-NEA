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

	glm::mat4 PerspectiveCamera::GetTransform()
	{
		constexpr glm::mat4 identity = glm::identity<glm::mat4>();

		glm::mat4 rotationMatrix = glm::rotate(identity, glm::radians(m_Rotation[0]), { 1.0f, 0.0f, 0.0f }) *
			glm::rotate(identity, glm::radians(m_Rotation[1]), { 0.0f, 1.0f, 0.0f }) *
			glm::rotate(identity, glm::radians(m_Rotation[2]), { 0.0f, 0.0f, 1.0f });

		return glm::translate(identity, m_Position) * rotationMatrix;
	}

	const glm::mat4 PerspectiveCamera::GetView()
	{
		return glm::inverse(GetTransform());
	}

}
