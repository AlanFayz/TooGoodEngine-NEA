#pragma once

#include <GLFW/glfw3.h>

namespace TooGoodEngine {
	
	enum class KeyCode
	{
		None = 0, Esc,
		One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Zero,
		Tab, Enter, Backspace, Space, Minus, Equal, LeftBracket, RightBracket, Backslash,
		Semicolon, Apostrophe, Comma, Period, Slash, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		PrintScreen, ScrollLock, Pause, Insert, Home, PageUp, Delete, End, PageDown, Right, Left, Down, Up,
		LeftControl, LeftShift, LeftAlt, LeftSuper, RightControl, RightShift, RightAlt, RightSuper, Menu,
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z
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