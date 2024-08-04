#include "Project.h"

#include <chrono>

namespace TooGoodEngine {

	Project::Project(const std::filesystem::path& path)
	{
		JsonReader reader(path);

		m_ProjectDirectory = std::filesystem::path(reader.Fetch<std::string>({ "Project Directory" }));
		m_ProjectName	   = reader.Fetch<std::string>({ "Project Name" });
	}

	Project::Project(const std::string& name, const std::filesystem::path& pathOfDirectory)
		: m_ProjectName(name), m_ProjectDirectory(pathOfDirectory)
	{
		if (m_ProjectDirectory.empty())
			m_ProjectDirectory = std::filesystem::current_path();

		SaveState();
	}

	Project::~Project()
	{
		SaveState();
	}

	void Project::SaveState()
	{
		std::filesystem::path file = m_ProjectDirectory / (m_ProjectName + ".json");
		JsonWriter writer(file);

		auto timePoint = std::chrono::system_clock::now();
		std::time_t currentTime = std::chrono::system_clock::to_time_t(timePoint);
		std::tm* nowTime = std::localtime(&currentTime);

		std::stringstream timeStream;
		timeStream << std::put_time(nowTime, "%Y-%m-%d %H:%M:%S");

		writer.WriteGeneric<std::string>({ "Project Name" }, m_ProjectName);
		writer.WriteGeneric<std::string>({ "Project Directory" }, m_ProjectDirectory.string());
		writer.WriteGeneric<std::string>({ "Last Build Date" }, timeStream.str());
	}
}