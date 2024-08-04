#pragma once

#include "Core/Files/Json.h"

namespace TooGoodEngine {
	
	class Project
	{
	public:
		Project(const std::filesystem::path& path); //for loading projects
		Project(const std::string& name, const std::filesystem::path& pathOfDirectory); //for creating
		~Project();

		void SaveState();

		inline const std::filesystem::path& GetDirectory() const { return m_ProjectDirectory; }

	private:
		std::filesystem::path m_ProjectDirectory;
		std::string m_ProjectName;
	};

	inline Ref<Project> g_SelecteProject = nullptr;
}