#include "Project.h"
#include "ComponentLoaders/ComponentLoader.h"
#include "ComponentWriter/ComponentWriter.h"
#include "Scripting/ScriptingEngine.h"
#include "Assets/Script.h"

#include <chrono>

namespace TooGoodEngine {

	const Project::ComponentLoaderMapType Project::s_ComponentLoaderMap = {
		{"Transform",		    [](EntityTree& registry, Entity& entity, const json& jsonComponent, Ref<Renderer> renderer) { registry.AddComponent(entity, ComponentLoader::LoadTransform(jsonComponent)); }},
		{"Mesh",			    [](EntityTree& registry, Entity& entity, const json& jsonComponent, Ref<Renderer> renderer) { registry.AddComponent(entity, ComponentLoader::LoadMesh(jsonComponent, *renderer)); } },
		{"Model",			    [](EntityTree& registry, Entity& entity, const json& jsonComponent, Ref<Renderer> renderer) { registry.AddComponent(entity, ComponentLoader::LoadModel(jsonComponent, *renderer)); } },
		{"Material",		    [](EntityTree& registry, Entity& entity, const json& jsonComponent, Ref<Renderer> renderer) { registry.AddComponent(entity, ComponentLoader::LoadMaterial(jsonComponent, renderer)); } },
		{"Point Light",		    [](EntityTree& registry, Entity& entity, const json& jsonComponent, Ref<Renderer> renderer) { registry.AddComponent(entity, ComponentLoader::LoadPointLight(jsonComponent)); } },
		{"Directional Light",   [](EntityTree& registry, Entity& entity, const json& jsonComponent, Ref<Renderer> renderer) { registry.AddComponent(entity, ComponentLoader::LoadDirectionalLight(jsonComponent)); } },
		{"Script",			    [](EntityTree& registry, Entity& entity, const json& jsonComponent, Ref<Renderer> renderer) { registry.AddComponent(entity, ComponentLoader::LoadScript(jsonComponent)); } },
		{"Perspective Camera",  [](EntityTree& registry, Entity& entity, const json& jsonComponent, Ref<Renderer> renderer) { registry.AddComponent(entity, ComponentLoader::LoadPerspectiveCamera(jsonComponent)); } },
		{"Orthographic Camera", [](EntityTree& registry, Entity& entity, const json& jsonComponent, Ref<Renderer> renderer) { registry.AddComponent(entity, ComponentLoader::LoadOrthographicCamera(jsonComponent)); } },

	};

	Project::Project(const std::filesystem::path& path)
	{
		JsonReader reader(path);

		m_ProjectDirectory = std::filesystem::path(reader.Fetch<std::string>({ "Project Directory" }));
		m_ProjectName	   = reader.Fetch<std::string>({ "Project Name" });

		m_AssetManager = CreateRef<AssetManager>(reader.Fetch<std::string>({"Asset Directory"}));

		ScriptingEngine::Init(m_AssetManager->GetPath());
	}

	Project::Project(const std::string& name, const std::filesystem::path& pathOfDirectory)
		: m_ProjectName(name), m_ProjectDirectory(pathOfDirectory)
	{
		if (m_ProjectDirectory.empty())
			m_ProjectDirectory = std::filesystem::current_path();

		m_CurrentScene = CreateRef<Scene>();
		m_LoadedScenes.push_back(m_CurrentScene);

		m_AssetManager = CreateRef<AssetManager>(m_ProjectDirectory / "Assets");

		ScriptingEngine::Init(m_AssetManager->GetPath());

		SaveState();
	}

	Project::~Project()
	{
		ScriptingEngine::Shutdown();
		ComponentLoader::ClearCaches();
	}

	void Project::SaveState(bool optimized)
	{
		std::filesystem::path file = m_ProjectDirectory / (m_ProjectName + ".json");
		JsonWriter writer(file, optimized); 

		//get the current time.
		auto timePoint = std::chrono::system_clock::now();
		std::time_t currentTime = std::chrono::system_clock::to_time_t(timePoint);
		std::tm* nowTime = std::localtime(&currentTime);

		std::stringstream timeStream;
		timeStream << std::put_time(nowTime, "%Y-%m-%d %H:%M:%S");

		writer.WriteGeneric({ "Project Name" },      m_ProjectName);
		writer.WriteGeneric({ "Project Directory" }, m_ProjectDirectory);
		writer.WriteGeneric({ "Asset Directory" },   m_AssetManager->GetPath());
		writer.WriteGeneric({ "Last Build Date" },   timeStream.str());

		for (const auto& scene: m_LoadedScenes)
			SaveScene(writer, scene);

		SaveAssets(writer);
	}

	void Project::Build(const std::filesystem::path& runtimeDirectory)
	{
		std::filesystem::copy(m_ProjectDirectory, runtimeDirectory, std::filesystem::copy_options::recursive | std::filesystem::copy_options::update_existing);

		std::filesystem::path file = runtimeDirectory / (m_ProjectName + ".json");
		JsonWriter writer(file, true); 

		auto timePoint = std::chrono::system_clock::now();
		std::time_t currentTime = std::chrono::system_clock::to_time_t(timePoint);
		std::tm* nowTime = std::localtime(&currentTime);

		std::stringstream timeStream;
		timeStream << std::put_time(nowTime, "%Y-%m-%d %H:%M:%S");

		writer.WriteGeneric({ "Project Name" },      m_ProjectName);
		writer.WriteGeneric({ "Project Directory" }, runtimeDirectory);
		writer.WriteGeneric({ "Asset Directory" },   runtimeDirectory / "Assets");
		writer.WriteGeneric({ "Last Build Date" },   timeStream.str());

		for (const auto& scene : m_LoadedScenes)
			SaveScene(writer, scene);

		SaveAssets(writer);
	}

	Ref<Scene> Project::LoadScene(const json& jsonScene, const std::string& name)
	{
		Ref<Scene> scene = CreateRef<Scene>();
		scene->SetName(name);

		LoadSceneSettings(scene, jsonScene);

		auto& registry = scene->GetRegistry();
		auto  renderer = scene->GetSceneRenderer();

		if (!jsonScene.contains("Entities"))
			return scene;

		auto& jsonEntities = jsonScene["Entities"];

		//first pass is to create all the entities and their components

		std::vector<Entity> entityList; //a copy

		for (auto it = jsonEntities.begin(); it != jsonEntities.end(); it++)
		{
			auto& jsonEntity = *it;
			Entity entity = registry.Add(it.key()); //create the entity and add it to the tree
			entityList.push_back(entity); //keep a copy for later use

			for(auto secondIt = jsonEntity.begin(); secondIt != jsonEntity.end(); secondIt++)
			{
				auto& jsonComponent = *secondIt;

				if (s_ComponentLoaderMap.contains(secondIt.key()))
					s_ComponentLoaderMap.at(secondIt.key())(registry, entity, jsonComponent, renderer);
			}
		}

		//order the entity heirarchy 
		uint64_t k = 0;
		for (const auto& jsonEntity : jsonEntities)
		{
			std::vector<std::string> children; 
			
			if(jsonEntity.contains("Children"))
				children = jsonEntity["Children"].get<std::vector<std::string>>();

			Entity& parent = entityList[k++];

			for (auto& child : children)
			{
				Entity childEntity = registry.GetEntityByName(child);
				if(childEntity)
					registry.Move(childEntity, parent);
			}
		}

		return scene;
	}

	void Project::LoadSceneSettings(Ref<Scene>& scene, const json& jsonScene)
	{
		RenderSettings renderSettings = scene->GetSceneRenderer()->GetSettings();

		auto& jsonSettings = jsonScene["Scene Settings"];

		renderSettings.DepthTesting  = (DepthTestOption)jsonSettings["Depth Testing"].get<int>();
		renderSettings.WindingOrder  = (WindingOrderOption)jsonSettings["Winding Order"].get<int>();
		renderSettings.Source        = (BlendingFactor)jsonSettings["Blending Source"].get<int>();
		renderSettings.Destination   = (BlendingFactor)jsonSettings["Blending Destination"].get<int>();
		renderSettings.FillingMode   = (FillMode)jsonSettings["Filling Mode"].get<int>();
		renderSettings.LevelOfDetail = jsonSettings["Level Of Detail"].get<float>();
		renderSettings.Gradient      = jsonSettings["Gradient"].get<float>();
		renderSettings.Bloom         = jsonSettings["Bloom Enabled"].get<bool>();
		renderSettings.Threshold     = jsonSettings["Bloom Threshold"].get<float>();
		renderSettings.Intensity     = jsonSettings["Bloom Intensity"].get<float>();
		renderSettings.FilterRadius  = jsonSettings["Bloom Filter Radius"].get<float>();

		std::array<float, 4> clearColor = jsonSettings["Clear Color"].get<std::array<float, 4>>();
		renderSettings.ClearColor = { clearColor[0], clearColor[1], clearColor[2], clearColor[3] };

		if (jsonSettings.contains("Environment Map"))
		{
			UUID id = jsonSettings["Environment Map"].get<uint64_t>();
			renderSettings.CurrentEnvironmentMap = m_AssetManager->FetchAssetAssuredType<EnvironmentMap>(id);
		}

		scene->GetSceneRenderer()->ChangeSettings(renderSettings);
	}

	void Project::SaveScene(JsonWriter& writer, const Ref<Scene>& scene)
	{
		SaveSceneSettings(writer, scene);

		auto& registry = scene->GetRegistry();

		//go through each entity in the registry.
		for (EntityID entityId = 0; entityId < registry.GetCount(); entityId++)
		{
			Entity entity = registry.GetEntityByID(entityId);

			//if its been deleted/invalid then ignore.
			if (entity.GetID() == g_NullEntity)
				continue;

			JsonPath pathToEntity = { "Scenes", scene->GetName(), "Entities", entity.GetName() };

			Node& node = registry.GetNode(entity);

			//if it has children then push back their names
			//into a vector and serilize them.
			if (!node.Children.empty())
			{
				JsonPath pathToChildren = pathToEntity;
				pathToChildren.push_back("Children");
				std::vector<std::string> strChildren;

				for (auto& child : node.Children)
				{
					Entity ent = scene->GetRegistry().GetEntityByID(child);
					if(ent)
						strChildren.push_back(ent.GetName());
				}

				writer.WriteGeneric(pathToChildren, strChildren);
			}

			//checks for a componenet, if it has a paticular component
			//then an associated write function is called serilizing that component.
			if (registry.HasComponent<TransformComponent>(entity))
				ComponentWriter::WriteTransform(writer, pathToEntity, registry.GetComponent<TransformComponent>(entity));

			if (registry.HasComponent<MeshComponent>(entity))
			{
				std::string path = registry.GetComponent<MeshComponent>(entity).PathToSource;
				MeshComponent writeComp;
				writeComp.PathToSource = path;
				ComponentWriter::WriteMesh(writer, pathToEntity, writeComp);
			}

			if (registry.HasComponent<MaterialComponent>(entity))
			{
				MaterialComponent& component = registry.GetComponent<MaterialComponent>(entity);
				ComponentWriter::WriteMaterial(writer, pathToEntity, component);
			}

			if (registry.HasComponent<PointLightComponent>(entity))
			{
				PointLightComponent& component = registry.GetComponent<PointLightComponent>(entity);
				ComponentWriter::WritePointLight(writer, pathToEntity, component);
			}

			if (registry.HasComponent<DirectionalLightComponent>(entity))
			{
				DirectionalLightComponent& component = registry.GetComponent<DirectionalLightComponent>(entity);
				ComponentWriter::WriteDirectionalLight(writer, pathToEntity, component);
			}

			if (registry.HasComponent<ModelComponent>(entity))
			{
				ModelComponent& component = registry.GetComponent<ModelComponent>(entity);
				ComponentWriter::WriteModel(writer, pathToEntity, component);
			}

			if (registry.HasComponent<ScriptComponent>(entity))
			{
				ScriptComponent& component = registry.GetComponent<ScriptComponent>(entity);
				ComponentWriter::WriteScript(writer, pathToEntity, component);
			}

			if (registry.HasComponent<PerspectiveCameraComponent>(entity))
			{
				PerspectiveCameraComponent& component = registry.GetComponent<PerspectiveCameraComponent>(entity);
				ComponentWriter::WritePerspectiveCamera(writer, pathToEntity, component);
			}

			if (registry.HasComponent<OrthographicCameraComponent>(entity))
			{
				OrthographicCameraComponent& component = registry.GetComponent<OrthographicCameraComponent>(entity);
				ComponentWriter::WriteOrthographicCamera(writer, pathToEntity, component);
			}

		}
	}

	void Project::SaveSceneSettings(JsonWriter& writer, const Ref<Scene>& scene)
	{
		//currently only contains renderer settings. this may change in the future.
		auto& renderSettings = scene->GetSceneRenderer()->GetSettings();
		std::string sceneName = scene->GetName();

		std::array<float, 4> clearColor = { renderSettings.ClearColor[0], renderSettings.ClearColor[1], renderSettings.ClearColor[2], renderSettings.ClearColor[3] };

		writer.WriteGeneric({ "Scenes", sceneName, "Scene Settings", "Depth Testing" },        (int)renderSettings.DepthTesting);
		writer.WriteGeneric({ "Scenes", sceneName, "Scene Settings", "Culling" },              (int)renderSettings.Culling);
		writer.WriteGeneric({ "Scenes", sceneName, "Scene Settings", "Winding Order" },        (int)renderSettings.WindingOrder);
		writer.WriteGeneric({ "Scenes", sceneName, "Scene Settings", "Blending Source" },      (int)renderSettings.Source);
		writer.WriteGeneric({ "Scenes", sceneName, "Scene Settings", "Blending Destination" }, (int)renderSettings.Destination);
		writer.WriteGeneric({ "Scenes", sceneName, "Scene Settings", "Filling Mode" },         (int)renderSettings.FillingMode);

		if (renderSettings.CurrentEnvironmentMap)
			writer.WriteGeneric({ "Scenes", sceneName, "Scene Settings", "Environment Map" }, (uint64_t)renderSettings.CurrentEnvironmentMap->GetAssetID());

		writer.WriteGeneric({ "Scenes", sceneName, "Scene Settings", "Level Of Detail" }, renderSettings.LevelOfDetail);
		writer.WriteGeneric({ "Scenes", sceneName, "Scene Settings", "Gradient" },		  renderSettings.Gradient);
		writer.WriteGeneric({ "Scenes", sceneName, "Scene Settings", "Clear Color" },     clearColor);
		
		writer.WriteGeneric({ "Scenes", sceneName, "Scene Settings", "Bloom Enabled" },       renderSettings.Bloom);
		writer.WriteGeneric({ "Scenes", sceneName, "Scene Settings", "Bloom Threshold" },     renderSettings.Threshold);
		writer.WriteGeneric({ "Scenes", sceneName, "Scene Settings", "Bloom Intensity" },     renderSettings.Intensity);
		writer.WriteGeneric({ "Scenes", sceneName, "Scene Settings", "Bloom Filter Radius" }, renderSettings.FilterRadius);
	}

	void Project::SaveAssets(JsonWriter& writer)
	{
		const auto& bank = m_AssetManager->GetBank();

		//go through each asset in the bank serilizing its ID, Type and Filepath.
		for (const auto& [assetUUID, asset] : bank)
		{
			std::string handle    = std::to_string((uint64_t)assetUUID);
			std::string_view type = GetAssetTypeString(asset->GetAssetType());
			std::filesystem::path relativePath = std::filesystem::relative(asset->GetPath(), m_AssetManager->GetPath());

			writer.WriteGeneric({ "Assets", handle, "Type" }, type);
			writer.WriteGeneric({ "Assets", handle, "Path" }, relativePath);
		}
	}

	void Project::LoadAssets(JsonReader& reader)
	{
		json assets = reader.Fetch<json>({ "Assets" });

		//go through each of the assets registering it with the asset manager.
		for (auto it = assets.begin(); it != assets.end(); it++)
		{
			auto& asset = *it;

			UUID id = std::stoull(it.key());

			AssetType type = GetAssetTypeFromString(asset["Type"].get<std::string>());
			std::filesystem::path path = asset["Path"].get<std::filesystem::path>();
			path = m_AssetManager->GetPath() / path;

			switch (type)
			{
				case AssetType::Image:			m_AssetManager->FetchAndLoadAssetWithID<Image>(path, id);		  break;
				case AssetType::Model:			m_AssetManager->FetchAndLoadAssetWithID<Model>(path, id);		  break;
				case AssetType::EnvironmentMap:  m_AssetManager->FetchAndLoadAssetWithID<EnvironmentMap>(path, id); break;
				case AssetType::Script:			m_AssetManager->FetchAndLoadAssetWithID<Script>(path, id);			break;
				case AssetType::None:
				default:
					break;
			}
		}	
	}

	void Project::LoadProject()
	{
		JsonReader reader(m_ProjectDirectory / (m_ProjectName + ".json"));

		LoadAssets(reader);

		auto scenes = reader.Fetch<json>({ "Scenes" });

		for (auto it = scenes.begin(); it != scenes.end(); it++)
		{
			m_CurrentScene = LoadScene(*it, it.key()); 

			if (m_CurrentScene)
				m_LoadedScenes.push_back(m_CurrentScene);
		}

		m_CurrentScene = m_LoadedScenes[0];
	}

	Ref<Project> Project::CreateProject(const std::filesystem::path& path)
	{
		g_SelectedProject = CreateRef<Project>(path);
		g_SelectedProject->LoadProject();

		return g_SelectedProject;
	}

	Ref<Project> Project::CreateNewProject(const std::string& name, const std::filesystem::path& directory)
	{
		g_SelectedProject = CreateRef<Project>(name, directory);
		return g_SelectedProject;
	}

	bool Project::ProjectLoaded()
	{
		return g_SelectedProject != nullptr;
	}

	void Project::RemoveSelectedProject()
	{
		g_SelectedProject.reset();
	}

}