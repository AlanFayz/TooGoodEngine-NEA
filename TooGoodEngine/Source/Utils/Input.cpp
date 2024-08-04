#include "Input.h"

#include "Core/Base.h"

namespace TooGoodEngine {

	GLFWwindow* Input::s_CurrentWindow = nullptr;

	void Input::Init(GLFWwindow* window)
	{
		TGE_VERIFY(!s_CurrentWindow && window, "already initialized or window is nullptr");

		s_CurrentWindow = window;
	}

	const bool Input::IsKeyReleased(KeyCode key)
	{
		TGE_VERIFY(s_CurrentWindow, "input wasn't initalized");
		return glfwGetKey(s_CurrentWindow, GetGLFWKeyCode(key)) == GLFW_RELEASE;
	}

	const bool Input::IsKeyPressed(KeyCode key)
	{
		TGE_VERIFY(s_CurrentWindow, "input wasn't initalized");
		return glfwGetKey(s_CurrentWindow, GetGLFWKeyCode(key)) == GLFW_PRESS;
	}

	const bool Input::IsKeyDown(KeyCode key)
	{
		TGE_VERIFY(s_CurrentWindow, "input wasn't initalized");
		int state = glfwGetKey(s_CurrentWindow, GetGLFWKeyCode(key));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	const bool Input::IsMouseButtonPressed(ButtonCode button)
	{
		TGE_VERIFY(s_CurrentWindow, "input wasn't initalized");
		return glfwGetMouseButton(s_CurrentWindow, GetGLFWButtonCode(button)) == GLFW_PRESS;
	}

	const bool Input::IsMouseButtonReleased(ButtonCode button)
	{
		TGE_VERIFY(s_CurrentWindow, "input wasn't initalized");
		return glfwGetMouseButton(s_CurrentWindow, GetGLFWButtonCode(button)) == GLFW_PRESS;
	}

	const bool Input::IsMouseButtonDown(ButtonCode button)
	{
		TGE_VERIFY(s_CurrentWindow, "input wasn't initalized");
		int state = glfwGetMouseButton(s_CurrentWindow, GetGLFWButtonCode(button));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
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

	constexpr int Input::GetGLFWKeyCode(KeyCode key)
	{
		switch (key)
		{
			case KeyCode::W:	return GLFW_KEY_W;
			case KeyCode::A:	return GLFW_KEY_A;
			case KeyCode::S:	return GLFW_KEY_S;
			case KeyCode::D:	return GLFW_KEY_D;
			case KeyCode::None: 
			default:	return 0;
		}
	}
	constexpr int Input::GetGLFWButtonCode(ButtonCode button)
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