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
			case KeyCode::Esc:        return GLFW_KEY_ESCAPE;
			case KeyCode::One:        return GLFW_KEY_1;
			case KeyCode::Two:        return GLFW_KEY_2;
			case KeyCode::Three:      return GLFW_KEY_3;
			case KeyCode::Four:       return GLFW_KEY_4;
			case KeyCode::Five:       return GLFW_KEY_5;
			case KeyCode::Six:        return GLFW_KEY_6;
			case KeyCode::Seven:      return GLFW_KEY_7;
			case KeyCode::Eight:      return GLFW_KEY_8;
			case KeyCode::Nine:       return GLFW_KEY_9;
			case KeyCode::Zero:       return GLFW_KEY_0;
			case KeyCode::Tab:        return GLFW_KEY_TAB;
			case KeyCode::Enter:      return GLFW_KEY_ENTER;
			case KeyCode::Backspace:  return GLFW_KEY_BACKSPACE;
			case KeyCode::Space:      return GLFW_KEY_SPACE;
			case KeyCode::Minus:      return GLFW_KEY_MINUS;
			case KeyCode::Equal:      return GLFW_KEY_EQUAL;
			case KeyCode::LeftBracket:return GLFW_KEY_LEFT_BRACKET;
			case KeyCode::RightBracket:return GLFW_KEY_RIGHT_BRACKET;
			case KeyCode::Backslash:  return GLFW_KEY_BACKSLASH;
			case KeyCode::Semicolon:  return GLFW_KEY_SEMICOLON;
			case KeyCode::Apostrophe: return GLFW_KEY_APOSTROPHE;
			case KeyCode::Comma:      return GLFW_KEY_COMMA;
			case KeyCode::Period:     return GLFW_KEY_PERIOD;
			case KeyCode::Slash:      return GLFW_KEY_SLASH;
			case KeyCode::F1:         return GLFW_KEY_F1;
			case KeyCode::F2:         return GLFW_KEY_F2;
			case KeyCode::F3:         return GLFW_KEY_F3;
			case KeyCode::F4:         return GLFW_KEY_F4;
			case KeyCode::F5:         return GLFW_KEY_F5;
			case KeyCode::F6:         return GLFW_KEY_F6;
			case KeyCode::F7:         return GLFW_KEY_F7;
			case KeyCode::F8:         return GLFW_KEY_F8;
			case KeyCode::F9:         return GLFW_KEY_F9;
			case KeyCode::F10:        return GLFW_KEY_F10;
			case KeyCode::F11:        return GLFW_KEY_F11;
			case KeyCode::F12:        return GLFW_KEY_F12;
			case KeyCode::PrintScreen:return GLFW_KEY_PRINT_SCREEN;
			case KeyCode::ScrollLock: return GLFW_KEY_SCROLL_LOCK;
			case KeyCode::Pause:      return GLFW_KEY_PAUSE;
			case KeyCode::Insert:     return GLFW_KEY_INSERT;
			case KeyCode::Home:       return GLFW_KEY_HOME;
			case KeyCode::PageUp:     return GLFW_KEY_PAGE_UP;
			case KeyCode::Delete:     return GLFW_KEY_DELETE;
			case KeyCode::End:        return GLFW_KEY_END;
			case KeyCode::PageDown:   return GLFW_KEY_PAGE_DOWN;
			case KeyCode::Right:      return GLFW_KEY_RIGHT;
			case KeyCode::Left:       return GLFW_KEY_LEFT;
			case KeyCode::Down:       return GLFW_KEY_DOWN;
			case KeyCode::Up:         return GLFW_KEY_UP;
			case KeyCode::LeftControl:return GLFW_KEY_LEFT_CONTROL;
			case KeyCode::LeftShift:  return GLFW_KEY_LEFT_SHIFT;
			case KeyCode::LeftAlt:    return GLFW_KEY_LEFT_ALT;
			case KeyCode::LeftSuper:  return GLFW_KEY_LEFT_SUPER;
			case KeyCode::RightControl:return GLFW_KEY_RIGHT_CONTROL;
			case KeyCode::RightShift: return GLFW_KEY_RIGHT_SHIFT;
			case KeyCode::RightAlt:   return GLFW_KEY_RIGHT_ALT;
			case KeyCode::RightSuper: return GLFW_KEY_RIGHT_SUPER;
			case KeyCode::Menu:       return GLFW_KEY_MENU;
			case KeyCode::A:          return GLFW_KEY_A;
			case KeyCode::B:          return GLFW_KEY_B;
			case KeyCode::C:          return GLFW_KEY_C;
			case KeyCode::D:          return GLFW_KEY_D;
			case KeyCode::E:          return GLFW_KEY_E;
			case KeyCode::F:          return GLFW_KEY_F;
			case KeyCode::G:          return GLFW_KEY_G;
			case KeyCode::H:          return GLFW_KEY_H;
			case KeyCode::I:          return GLFW_KEY_I;
			case KeyCode::J:          return GLFW_KEY_J;
			case KeyCode::K:          return GLFW_KEY_K;
			case KeyCode::L:          return GLFW_KEY_L;
			case KeyCode::M:          return GLFW_KEY_M;
			case KeyCode::N:          return GLFW_KEY_N;
			case KeyCode::O:          return GLFW_KEY_O;
			case KeyCode::P:          return GLFW_KEY_P;
			case KeyCode::Q:          return GLFW_KEY_Q;
			case KeyCode::R:          return GLFW_KEY_R;
			case KeyCode::S:          return GLFW_KEY_S;
			case KeyCode::T:          return GLFW_KEY_T;
			case KeyCode::U:          return GLFW_KEY_U;
			case KeyCode::V:          return GLFW_KEY_V;
			case KeyCode::W:          return GLFW_KEY_W;
			case KeyCode::X:          return GLFW_KEY_X;
			case KeyCode::Y:          return GLFW_KEY_Y;
			case KeyCode::Z:          return GLFW_KEY_Z;
			case KeyCode::None:      
			default:                 return 0;
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