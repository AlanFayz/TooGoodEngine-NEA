#pragma once

#include <inttypes.h>
#include <type_traits>

namespace TooGoodEngine {

	enum class EventType
	{
		None = 0, WindowResize, ApplicationClose
	};

	class Event
	{
	public:
		virtual EventType GetType() const = 0;
	};

	//note: events are not marked as "handled"
	//they are passed to the event systems OnEvent once

	class EventHandler
	{
	public:
		virtual void OnEvent(Event* event) = 0;
	};

	template<typename T>
	class EventDispatcher
	{
	public:
		EventDispatcher(T* context) : m_EventHandler(context) {};
		~EventDispatcher() = default;

		template<typename E>
		void Dispatch(E* event)
		{
			static_assert(std::is_base_of_v<EventHandler, T>, "not base class of event handler");
			static_assert(std::is_base_of_v<Event, E>, "not a base class of event");

			m_EventHandler->OnEvent(event);
		}

	private:
		T* m_EventHandler;
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t width, uint32_t height)
			: m_Width(width), m_Height(height)
		{
		}

		~WindowResizeEvent() = default;

		inline const uint32_t GetWidth() const { return m_Width; }
		inline const uint32_t GetHeight() const { return m_Height; }

		virtual EventType GetType() const { return EventType::WindowResize; }
	private:
		uint32_t m_Width, m_Height;
	};

	class ApplicationCloseEvent : public Event
	{
	public:
		ApplicationCloseEvent() = default;
		~ApplicationCloseEvent() = default;

		virtual EventType GetType() const { return EventType::ApplicationClose; }
	};


}