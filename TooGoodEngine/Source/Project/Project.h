#pragma once

#include "Core/Files/Json.h"
#include "Assets/AssetManager.h"
#include "Scenes/Scene.h"

namespace TooGoodEngine {
	
	class Project
	{
	public:
		Project(const std::filesystem::path& path); //for loading projects
		Project(const std::string& name, const std::filesystem::path& pathOfDirectory); //for creating
		~Project();

		void SaveState();

		template<typename T>
		Ref<T> LoadAsset(const std::filesystem::path& path) 
		{ 
			return m_AssetManager.LoadAssetIntoBank<T>(path);
		}

		inline const std::filesystem::path& GetDirectory() const { return m_ProjectDirectory; }

	private:
		std::filesystem::path m_ProjectDirectory;
		std::string m_ProjectName;

		AssetManager m_AssetManager; 
		std::vector<Ref<Scene>> m_LoadedScenes;
	};

	inline Ref<Project> g_SelecteProject = nullptr;
}