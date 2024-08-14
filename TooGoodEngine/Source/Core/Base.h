#pragma once

#include <memory>
#include <iostream>

namespace TooGoodEngine {

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	using WeakRef   = std::weak_ptr<T>;

	template<typename T>
	using Scoped = std::unique_ptr<T>;

	template<typename T, typename ...Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename ...Args>
	constexpr Scoped<T> CreateScoped(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	struct TextColor
	{
		static constexpr const char* Reset = "\033[0m";
		static constexpr const char* Black = "\033[0;30m";
		static constexpr const char* Red = "\033[0;31m";
		static constexpr const char* Green = "\033[0;32m";
		static constexpr const char* Yellow = "\033[0;33m";
		static constexpr const char* Blue = "\033[0;34m";
		static constexpr const char* Magenta = "\033[0;35m";
		static constexpr const char* Cyan = "\033[0;36m";
		static constexpr const char* White = "\033[0;37m";
	};

	template<typename ...Args>
	void Log(const char* Color, Args&&... args)
	{
		std::cout << Color;
		(std::cout << ... << std::forward<Args>(args));
		std::cout << TextColor::Reset << std::endl;
	}

	#define TGE_LOG_INFO(...)    TooGoodEngine::Log(TextColor::Green,  "Engine Info: ",    __VA_ARGS__)
	#define TGE_LOG_WARNING(...) TooGoodEngine::Log(TextColor::Yellow, "Engine Warning: ", __VA_ARGS__)
	#define TGE_LOG_ERROR(...)   TooGoodEngine::Log(TextColor::Red,    "Engine Error: ",   __VA_ARGS__)

	#define TGE_HALT() __debugbreak()
	
	#ifdef DEBUG
		#define TGE_ASSERT(condition, ...) if(!(condition)) {TGE_LOG_ERROR(__VA_ARGS__); TGE_HALT();}
	#else 
		#define TGE_ASSERT(condition, ...) 
	#endif

	#ifdef RELEASE 
		#define TGE_VERIFY(condition, ...) if(!(condition)) {TGE_LOG_ERROR(__VA_ARGS__); TGE_HALT();}
	#elif DEBUG
		#define TGE_VERIFY(condition, ...) if(!(condition)) {TGE_LOG_ERROR(__VA_ARGS__); TGE_HALT();}
	#else 
		#define TGE_VERIFY(condition, ...) 
	#endif
}