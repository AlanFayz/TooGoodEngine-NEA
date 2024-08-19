#pragma once

#include <GLFW/glfw3.h>

namespace TooGoodEngine {
	
	enum class KeyCode
	{
		None = 0, W, A, S, D, Q, E, X, Esc //will add rest later
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

		static void GetMouseCoordinates(double& x, double& y);
		static void GetScrollWheel(double& x, double& y);

		static void DisableCursor();
		static void EnableCursor();

	private:
		static constexpr int _GetGLFWKeyCode(KeyCode key);
		static constexpr int _GetGLFWButtonCode(ButtonCode button);

		static void _ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

	private:
		static GLFWwindow* s_CurrentWindow;

		static double s_CurrentXOffset;
		static double s_CurrentYOffset;
		
	};
}