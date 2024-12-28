#include "OrthographicCamera.h"


namespace TooGoodEngine {

	OrthographicCamera::OrthographicCamera(const OrthographicCameraData& data)
		: m_Position(data.Position), m_Rotation(data.Rotation),
		  m_Top(data.Top), m_Bottom(data.Bottom), m_Left(data.Left), m_Right(data.Right)
	{
	}

	void OrthographicCamera::SetData(const OrthographicCameraData& data)
	{
		m_Position = data.Position;
		m_Rotation = data.Rotation;

		m_Top = data.Top;
		m_Bottom = data.Bottom;
		m_Left = data.Left;
		m_Right = data.Right;
	}

	void OrthographicCamera::OnWindowResize(float newWidth, float newHeight)
	{
		m_Left  = -newWidth / 200.0f;
		m_Right = newWidth / 200.0f;
		m_Bottom = -newHeight / 200.0f;
		m_Top   = newHeight / 200.0f;
	}

	glm::mat4 OrthographicCamera::GetTransform()
	{
		constexpr glm::mat4 identity = glm::identity<glm::mat4>();

		glm::mat4 rotationMatrix = glm::rotate(identity, glm::radians(m_Rotation[0]), { 1.0f, 0.0f, 0.0f }) *
			glm::rotate(identity, glm::radians(m_Rotation[1]), { 0.0f, 1.0f, 0.0f }) *
			glm::rotate(identity, glm::radians(m_Rotation[2]), { 0.0f, 0.0f, 1.0f });

		return glm::translate(identity, m_Position) * rotationMatrix;
	}
}
