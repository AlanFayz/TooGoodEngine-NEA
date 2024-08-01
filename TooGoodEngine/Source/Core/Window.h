#pragma once

#include "Core/Base.h"
#include "Events/Event.h"

#include <GLFW/glfw3.h>

namespace TooGoodEngine {

	class Application;

	class Window
	{
	public:
		Window(uint32_t width, uint32_t height, const std::string& title, EventDispatcher<Application>& appDispatcher);
		~Window();

		void Update();

		inline const uint32_t GetWidth()  const { return m_Width; }
		inline const uint32_t GetHeight() const { return m_Height; }
		
		inline void SetWidth(uint32_t width)   { m_Width = width; }
		inline void SetHeight(uint32_t height) { m_Height = height; }


		inline EventDispatcher<Application>& GetDispatcher() { return m_Dispatcher; }
		inline GLFWwindow* GetWindow() const { return m_Window; }

	private:
		uint32_t m_Width, m_Height;
		std::string m_Title;
		GLFWwindow* m_Window;
		EventDispatcher<Application> m_Dispatcher;
	};

}