#include "Project.h"
#include "ComponentLoaders/ComponentLoader.h"
#include "ComponentWriter/ComponentWriter.h"

#include <chrono>

namespace TooGoodEngine {

	Project::Project(const std::filesystem::path& path)
	{
		JsonReader reader(path);

		m_ProjectDirectory = std::filesystem::path(reader.Fetch<std::string>({ "Project Directory" }));
		m_ProjectName	   = reader.Fetch<std::string>({ "Project Name" });

		auto scenes = reader.Fetch<json>({ "Scenes" });

		for (auto it = scenes.begin(); it != scenes.end(); it++)
		{
			m_CurrentScene = LoadScene(*it, it.key()); //first scene will be start scene
		
			if (m_CurrentScene)
				m_LoadedScenes.push_back(m_CurrentScene);
		}

		m_CurrentScene = m_LoadedScenes[0];
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
		JsonWriter writer(file, false); //will be true when build() function is created

		auto timePoint = std::chrono::system_clock::now();
		std::time_t currentTime = std::chrono::system_clock::to_time_t(timePoint);
		std::tm* nowTime = std::localtime(&currentTime);

		std::stringstream timeStream;
		timeStream << std::put_time(nowTime, "%Y-%m-%d %H:%M:%S");

		writer.WriteGeneric<std::string>({ "Project Name" },     m_ProjectName);
		writer.WriteGeneric<std::string>({ "Project Directory" }, m_ProjectDirectory.string());
		writer.WriteGeneric<std::string>({ "Last Build Date" }, timeStream.str());

		auto& registry = m_CurrentScene->GetRegistry();

		//TODO: will need to do for multiple scenes when supported

		for (EntityID entityId = 0; entityId < registry.GetCount(); entityId++)
		{
			Entity entity = registry.GetEntity(entityId);

			JsonPath pathToEntity = { "Scenes", m_CurrentScene->GetName(), "Entities", entity.GetName()};


			Node& node = registry.GetNode(entity);
			
			if (!node.Children.empty())
			{
				JsonPath pathToChildren = pathToEntity;
				pathToChildren.push_back("Children");
				writer.WriteGeneric(pathToChildren, node.Children);
			}

			if (registry.HasComponent<TransformComponent>(entity))
				ComponentWriter::WriteTransform(writer, pathToEntity, registry.GetComponent<TransformComponent>(entity));

			if (registry.HasComponent<MeshComponent>(entity))
			{
				std::string path = registry.GetComponent<MeshComponent>(entity).PathToSource;
				MeshComponent writeComp;
				writeComp.PathToSource = path;
				ComponentWriter::WriteMesh(writer, pathToEntity, writeComp);
			}

		}
	}

	Ref<Scene> Project::LoadScene(const json& jsonScene, const std::string& name)
	{
		Ref<Scene> scene = CreateRef<Scene>();
	
		scene->SetName(name);

		auto& registry = scene->GetRegistry();
		auto  renderer = scene->GetSceneRenderer();

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

				if (secondIt.key() == "Transform")
					registry.AddComponent<TransformComponent>(entity, ComponentLoader::LoadTransform(jsonComponent));
				
				else if (secondIt.key() == "Mesh")
				{
					MeshComponent component = ComponentLoader::LoadMesh(jsonComponent, *renderer);
					registry.AddComponent<MeshComponent>(entity, component);
				}
			}
		}

		//order the heirarchy in the tree
		uint64_t k = 0;
		for (const auto& jsonEntity : jsonEntities)
		{
			std::vector<EntityID> children; 
			
			if(jsonEntity.contains("Children"))
				children = jsonEntity["Children"].get<std::vector<EntityID>>();

			Entity& parent = entityList[k++];

			for (auto& child : children)
			{
				Entity childEntity("loop entity", child); //all that matters is the ID here name is for debug
				registry.Move(childEntity, parent);
			}
		}

		return scene;
	}

}