#pragma once

#include "Core/Files/Json.h"
#include "Assets/AssetManager.h"
#include "Scenes/Scene.h"

namespace TooGoodEngine {

	/*
		Contains most of the serilization and desirlization of projects
		from json files. There will be a runtime version of this which will 
		be be built from the editor and simply dump data as is (in binary)
		in files and have a optimized json file for locations to all.
	*/
	
	class Project
	{
	public:
		Project(const std::filesystem::path& path); //for loading projects
		Project(const std::string& name, const std::filesystem::path& pathOfDirectory); //for creating
		~Project();

		void SaveState();

		Ref<Scene> LoadScene(const json& jsonScene, const std::string& name);
		void SaveScene(JsonWriter& writer, const Ref<Scene>& scene);
		void SaveAssets(JsonWriter& writer);
		void LoadAssets(JsonReader& reader);
		void LoadAllScenes();

		template<typename T>
		Ref<T> LoadAsset(const std::filesystem::path& path) 
		{ 
			return m_AssetManager->LoadAssetIntoBank<T>(path);
		}

		//TODO: change this to be fetched from project file (asset directory)

		inline const std::filesystem::path  GetAssetDirectory() const { return m_AssetManager->GetPath(); }
		inline const std::filesystem::path& GetDirectory() const { return m_ProjectDirectory; }
		inline Ref<Scene> GetCurrentScene() const { return m_CurrentScene; }
		inline AssetManager& GetAssetManager() { return *m_AssetManager; }

	private:
		std::filesystem::path m_ProjectDirectory;
		std::string m_ProjectName;

		Ref<AssetManager> m_AssetManager; 
		Ref<Scene> m_CurrentScene = nullptr;
		std::vector<Ref<Scene>> m_LoadedScenes;
	};

	inline Ref<Project> g_SelectedProject = nullptr;
}