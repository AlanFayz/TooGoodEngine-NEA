#include "Statistics.h"

namespace TooGoodEngine {

	std::map<std::string_view, double> Statistics::m_StatisticsMap;

	void Statistics::Add(const std::string_view& identifier, double data)
	{
		m_StatisticsMap[identifier] = data;
	}

	double Statistics::Get(const std::string_view& identifier)
	{
		return m_StatisticsMap[identifier];
	}

	ProfileScope::ProfileScope(const std::string_view& name)
		: m_Name(name)
	{
		m_Timer.Start();
	}
	ProfileScope::~ProfileScope()
	{
		Statistics::Add(m_Name, (double)m_Timer.EllapsedMilli());
	}
}
