#include "Input.h"

#include "Core/Base.h"

namespace TooGoodEngine {

	GLFWwindow* Input::s_CurrentWindow = nullptr;
	
	double Input::s_CurrentXOffset = 1.0;
	double Input::s_CurrentYOffset = 1.0;

	void Input::Init(GLFWwindow* window)
	{
		TGE_VERIFY(!s_CurrentWindow && window, "already initialized or window is nullptr");
		s_CurrentWindow = window;

		glfwSetScrollCallback(s_CurrentWindow, _ScrollCallback);
	}

	const bool Input::IsKeyReleased(KeyCode key)
	{
		TGE_VERIFY(s_CurrentWindow, "input wasn't initalized");
		return glfwGetKey(s_CurrentWindow, _GetGLFWKeyCode(key)) == GLFW_RELEASE;
	}

	const bool Input::IsKeyPressed(KeyCode key)
	{
		TGE_VERIFY(s_CurrentWindow, "input wasn't initalized");
		return glfwGetKey(s_CurrentWindow, _GetGLFWKeyCode(key)) == GLFW_PRESS;
	}

	const bool Input::IsKeyDown(KeyCode key)
	{
		TGE_VERIFY(s_CurrentWindow, "input wasn't initalized");
		int state = glfwGetKey(s_CurrentWindow, _GetGLFWKeyCode(key));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	const bool Input::IsMouseButtonPressed(ButtonCode button)
	{
		TGE_VERIFY(s_CurrentWindow, "input wasn't initalized");
		return glfwGetMouseButton(s_CurrentWindow, _GetGLFWButtonCode(button)) == GLFW_PRESS;
	}

	const bool Input::IsMouseButtonReleased(ButtonCode button)
	{
		TGE_VERIFY(s_CurrentWindow, "input wasn't initalized");
		return glfwGetMouseButton(s_CurrentWindow, _GetGLFWButtonCode(button)) == GLFW_PRESS;
	}

	const bool Input::IsMouseButtonDown(ButtonCode button)
	{
		TGE_VERIFY(s_CurrentWindow, "input wasn't initalized");
		int state = glfwGetMouseButton(s_CurrentWindow, _GetGLFWButtonCode(button));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	void Input::GetMouseCoordinates(double& x, double& y)
	{
		TGE_VERIFY(s_CurrentWindow, "input wasn't initalized");
		glfwGetCursorPos(s_CurrentWindow, &x, &y);
	}

	void Input::GetScrollWheel(double& x, double& y)
	{
		TGE_VERIFY(s_CurrentWindow, "input wasn't initalized");
		x = s_CurrentXOffset;
		y = s_CurrentYOffset;
	}

	void Input::DisableCursor()
	{
		TGE_VERIFY(s_CurrentWindow, "input wasn't initalized");
		glfwSetInputMode(s_CurrentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void Input::EnableCursor()
	{
		TGE_VERIFY(s_CurrentWindow, "input wasn't initalized");
		glfwSetInputMode(s_CurrentWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void Input::_ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		s_CurrentXOffset = xOffset;
		s_CurrentYOffset = yOffset;
	}

	constexpr int Input::_GetGLFWKeyCode(KeyCode key)
	{
		switch (key)
		{
			case KeyCode::W:	return GLFW_KEY_W;
			case KeyCode::A:	return GLFW_KEY_A;
			case KeyCode::S:	return GLFW_KEY_S;
			case KeyCode::D:	return GLFW_KEY_D;
			case KeyCode::Q:    return GLFW_KEY_Q;
			case KeyCode::E:    return GLFW_KEY_E;
			case KeyCode::X:	return GLFW_KEY_X;
			case KeyCode::None: 
			default:	return 0;
		}
	}
	constexpr int Input::_GetGLFWButtonCode(ButtonCode button)
	{
		switch (button)
		{
			case ButtonCode::LeftMouse:		return GLFW_MOUSE_BUTTON_LEFT;
			case ButtonCode::RightMouse:	return GLFW_MOUSE_BUTTON_RIGHT;
			case ButtonCode::None:
			default:	return 0;
		}
	
	}
}