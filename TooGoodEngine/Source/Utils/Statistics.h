#pragma once

#include <map>
#include <string_view>
#include "Timer.h"

namespace TooGoodEngine {

	class Statistics
	{
	public:
		static void Add(const std::string_view& identifier, double data);
		static double Get(const std::string_view& identifier);
		static const auto& GetMap() { return m_StatisticsMap; }

	private:
		static std::map<std::string_view, double> m_StatisticsMap;
	};

	class ProfileScope
	{
	public:
		ProfileScope(const std::string_view& name);
		~ProfileScope();

	private:
		Timer m_Timer;
		std::string_view m_Name;
	};

	
	#ifdef DEBUG
		#define TGE_PROFILE_SCOPE(name) TooGoodEngine::ProfileScope name(#name); 
	#elif RELEASE
		#define TGE_PROFILE_SCOPE(name) TooGoodEngine::ProfileScope name(#name); 
	#else 
		#define TGE_PROFILE_SCOPE(name)
	#endif
}