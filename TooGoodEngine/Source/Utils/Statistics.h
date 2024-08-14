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

	//defined here just incase I may want to disable in dist builds as timers do 
	//take up a considerable amount of time.
	#define TGE_PROFILE_SCOPE(name) TooGoodEngine::ProfileScope name(#name); 
}