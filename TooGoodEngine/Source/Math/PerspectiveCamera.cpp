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
		return glm::translate(identity, m_Position) * GetOrientation();
	}

	glm::mat4 PerspectiveCamera::GetOrientation()
	{
		constexpr glm::mat4 identity = glm::identity<glm::mat4>();

		return glm::rotate(identity, glm::radians(m_Rotation[2]), { 0.0f, 0.0f, 1.0f }) *  
			glm::rotate(identity, glm::radians(m_Rotation[0]), { 1.0f, 0.0f, 0.0f }) * 
			glm::rotate(identity, glm::radians(m_Rotation[1]), { 0.0f, 1.0f, 0.0f });   
	}

	const glm::mat4 PerspectiveCamera::GetView()
	{
		glm::vec3 forward = glm::normalize(glm::vec3(
			glm::sin(m_Rotation.y) * glm::cos(m_Rotation.x),
			glm::sin(m_Rotation.x),
			glm::cos(m_Rotation.y) * glm::cos(m_Rotation.x)
		));

		glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

		glm::vec3 up = glm::cross(right, forward);

		glm::vec3 target = m_Position + forward;

		return glm::lookAt(m_Position, target, up);
	}



}
