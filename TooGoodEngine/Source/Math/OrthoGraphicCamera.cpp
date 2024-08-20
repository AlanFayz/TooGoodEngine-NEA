#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace TooGoodEngine {

	OrthographicCamera::OrthographicCamera(const OrthographicCameraData& data)
		: m_View(), m_Projection(), m_InverseView(), m_InverseProjection(), 
		  m_Position(data.Position), m_Front(data.Front), m_Up(data.Up),
		  m_Top(data.Top), m_Bottom(data.Bottom), m_Left(data.Left), m_Right(data.Right)
	{
		UpdateViewProjection();
	}

	void OrthographicCamera::SetData(const OrthographicCameraData& data)
	{
		m_Position = data.Position;
		m_Front = data.Front;
		m_Up = data.Up;
		m_Top = data.Top;
		m_Bottom = data.Bottom;
		m_Left = data.Left;
		m_Right = data.Right;

		UpdateViewProjection();
	}

	void OrthographicCamera::OnWindowResize(float newWidth, float newHeight)
	{
		m_Left  = -newWidth / 200.0f;
		m_Right = newWidth / 200.0f;
		m_Bottom = -newHeight / 200.0f;
		m_Top   = newHeight / 200.0f;

		UpdateViewProjection();
	}
	void OrthographicCamera::UpdateViewProjection()
	{
		m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		m_Projection = glm::ortho(m_Left, m_Right, m_Bottom, m_Top, 0.1f, 100.0f);

		m_InverseView = glm::inverse(m_View);
		m_InverseProjection = glm::inverse(m_Projection);
	}
}
