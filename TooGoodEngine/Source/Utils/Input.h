#pragma once

#include <GLFW/glfw3.h>

namespace TooGoodEngine {
	
	enum class KeyCode
	{
		None = 0, W, A, S, D //will add rest later
	};

	enum class ButtonCode
	{
		None = 0, LeftMouse, RightMouse //same as above 
	};

	class Input
	{
	public:
		static void Init(GLFWwindow* window);

		static const bool IsKeyReleased(KeyCode key);
		static const bool IsKeyPressed(KeyCode key);
		static const bool IsKeyDown(KeyCode key);

		static const bool IsMouseButtonPressed(ButtonCode button);
		static const bool IsMouseButtonReleased(ButtonCode button);
		static const bool IsMouseButtonDown(ButtonCode button);

		static void DisableCursor();
		static void EnableCursor();

	private:
		static constexpr int GetGLFWKeyCode(KeyCode key);
		static constexpr int GetGLFWButtonCode(ButtonCode button);

	private:
		static GLFWwindow* s_CurrentWindow;
	};
}