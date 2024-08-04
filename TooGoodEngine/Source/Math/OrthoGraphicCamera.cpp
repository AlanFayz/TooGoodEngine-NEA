#include "OrthoGraphicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace TooGoodEngine {

	OrthoGraphicCamera::OrthoGraphicCamera(const OrthoGraphicCameraData& data)
		: m_View(), m_Projection(), m_InverseView(), m_InverseProjection(), 
		  m_Position(data.Position), m_Front(data.Front), m_Up(data.Up),
		  m_Top(data.Top), m_Bottom(data.Bottom), m_Left(data.Left), m_Right(data.Right)
	{
		m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		m_Projection = glm::ortho(m_Left, m_Right, m_Bottom, m_Top);

		m_InverseView = glm::inverse(m_View);
		m_InverseProjection = glm::inverse(m_Projection);
	}

	void OrthoGraphicCamera::SetData(const OrthoGraphicCameraData& data)
	{
		m_Position = data.Position;
		m_Front = data.Front;
		m_Up = data.Up;
		m_Top = data.Top;
		m_Bottom = data.Bottom;
		m_Left = data.Left;
		m_Right = data.Right;

		m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		m_Projection = glm::ortho(m_Left, m_Right, m_Bottom, m_Top);

		m_InverseView = glm::inverse(m_View);
		m_InverseProjection = glm::inverse(m_Projection);
	}
}
