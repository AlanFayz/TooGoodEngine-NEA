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

	class EventDispatcher
	{
	public:
		EventDispatcher() = default;
		~EventDispatcher() = default;

		template<typename Type, typename E>
		void Dispatch(Type* eventHandler, E* event)
		{
			static_assert(std::is_base_of_v<EventHandler, Type>, "not base class of event handler");
			static_assert(std::is_base_of_v<Event, E>, "not a base class of event");

			eventHandler->OnEvent(event);
		}
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