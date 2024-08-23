#include "ComponentLoader.h"

#include "Project/Project.h"
#include "Scripting/ScriptingEngine.h"
#include "Assets/Script.h"


namespace TooGoodEngine {
		
	Ref<ModelCache> ComponentLoader::m_ModelCache = CreateRef<ModelCache>();

	TransformComponent ComponentLoader::LoadTransform(const json& jsonComponent)
	{
		TransformComponent component{};

		auto& componentPosition = component.GetPosition();
		auto& componentRotation = component.GetRotation();
		auto& componentScale    = component.GetScale();

		std::vector<float> position = jsonComponent["Position"].get<std::vector<float>>();
		std::vector<float> scale = jsonComponent["Scale"].get<std::vector<float>>();
		std::vector<float> rotation = jsonComponent["Rotation"].get<std::vector<float>>();

		int i = 0;
		for (; i < position.size(); i++)
		{
			if (i >= 3) //too many
				break;

			componentPosition[i] = position[i];
		}

		i = 0;
		for (; i < scale.size(); i++)
		{
			if (i >= 3) //too many
				break;

			componentScale[i] = scale[i];
		}

		i = 0;
		for (; i < rotation.size(); i++)
		{
			if (i >= 3) //too many
				break;

			componentRotation[i] = rotation[i];
		}


		component.UpdateTransform();

		return component;
	}
	
	MeshComponent ComponentLoader::LoadMesh(const json& jsonComponent, Renderer& sceneRenderer)
	{
		MeshComponent component;

		//there may be more primitves in the future added here

		if (jsonComponent.get<std::string>() == "##Quad") //quad
		{
			component.ID = 0;
			component.PathToSource = "##Quad";
			return component;
		}
		else if (jsonComponent.get<std::string>() == "##Cube")
		{
			component.ID = 1;
			component.PathToSource = "##Cube";

			return component;
		}
		
		return component;
	}

	ModelComponent ComponentLoader::LoadModel(const json& jsonComponent, Renderer& sceneRenderer)
	{
		ModelComponent component{};

		auto& cache = *m_ModelCache;

		component.ModelAssetId = jsonComponent.get<uint64_t>();

		if (cache.contains(component.ModelAssetId))
		{
			component.Info = cache[component.ModelAssetId];
			return component;
		}

		Ref<Model> model = g_SelectedProject->GetAssetManager().FetchAssetAssuredType<Model>(component.ModelAssetId);
		component.Info = sceneRenderer.AddModel(model);

		cache[component.ModelAssetId] = component.Info;

		return component;
	}

	MaterialComponent ComponentLoader::LoadMaterial(const json& jsonComponent, Renderer& sceneRenderer)
	{
		MaterialInfo info{};

		//
		// ---- Ambient ----
		if (jsonComponent.contains("Ambient Image"))
		{
			UUID handle = jsonComponent["Ambient Image"].get<uint64_t>();
			info.AmbientTexture = g_SelectedProject->GetAssetManager().FetchAssetAssuredType<Image>(handle);
		}
		
		{
			std::array<float, 4> ambientCol = jsonComponent["Ambient"].get<std::array<float, 4>>();
			info.Ambient = { ambientCol[0], ambientCol[1], ambientCol[2], ambientCol[3] };
		}

		//
		// ---- Albedo ---- 
		if (jsonComponent.contains("Albedo Image"))
		{
			UUID handle = jsonComponent["Albedo Image"].get<uint64_t>();
			info.AlbedoTexture = g_SelectedProject->GetAssetManager().FetchAssetAssuredType<Image>(handle);
		}

		{
			std::array<float, 4> albedoCol = jsonComponent["Albedo"].get<std::array<float, 4>>();
			info.Albedo = { albedoCol[0], albedoCol[1], albedoCol[2], albedoCol[3] };
		}

		//
		// ---- Metallic ----
		if (jsonComponent.contains("Metallic Image"))
		{
			UUID handle = jsonComponent["Metallic Image"].get<uint64_t>();
			info.MetallicTexture = g_SelectedProject->GetAssetManager().FetchAssetAssuredType<Image>(handle);
		}

		{
			info.Metallic.x = jsonComponent["Metallic"].get<float>();
		}


		//
		// ---- Emission ----
		if (jsonComponent.contains("Emission Image"))
		{
			UUID handle = jsonComponent["Emission Image"].get<uint64_t>();
			info.EmissionTexture = g_SelectedProject->GetAssetManager().FetchAssetAssuredType<Image>(handle);
		}

		{
			std::array<float, 4> emissionCol = jsonComponent["Emission"].get<std::array<float, 4>>();
			info.Emission = { emissionCol[0], emissionCol[1], emissionCol[2], emissionCol[3] };
		}

		info.EmissionFactor = jsonComponent["Emission Factor"].get<float>();

		//
		// ---- Roughness ----
		if (jsonComponent.contains("Roughness Image"))
		{
			UUID handle = jsonComponent["Roughness Image"].get<uint64_t>();
			info.RoughnessTexture = g_SelectedProject->GetAssetManager().FetchAssetAssuredType<Image>(handle);
		}

		{
			info.Roughness = jsonComponent["Roughness"].get<float>();
		}

		MaterialComponent component{};
		component.Material = CreateMaterial(info);
		component.ID = sceneRenderer.AddMaterial(component.Material);

		return component;
	}

	PointLightComponent ComponentLoader::LoadPointLight(const json& jsonComponent)
	{
		PointLightComponent component{};
		
		std::array<float, 4> color = jsonComponent["Color"].get<std::array<float, 4>>();
		component.Color = { color[0], color[1], color[2], color[3] };

		std::array<float, 3> position = jsonComponent["Position"].get<std::array<float, 3>>();
		component.Position = { position[0], position[1], position[2] };

		component.Radius = jsonComponent["Radius"].get<float>();
		component.Intensity = jsonComponent["Intensity"].get<float>();

		return component;
	}

	DirectionalLightComponent ComponentLoader::LoadDirectionalLight(const json& jsonComponent)
	{
		DirectionalLightComponent component{};

		std::array<float, 4> color = jsonComponent["Color"].get<std::array<float, 4>>();
		component.Color = { color[0], color[1], color[2], color[3] };

		std::array<float, 3> direction = jsonComponent["Direction"].get<std::array<float, 3>>();
		component.Direction = { direction[0], direction[1], direction[2] };

		component.Intensity = jsonComponent["Intensity"].get<float>();

		return component;
	}

	ScriptComponent ComponentLoader::LoadScript(const json& jsonComponent)
	{
		ScriptComponent component;

		UUID id = jsonComponent.get<uint64_t>();
		Ref<Script> asset = g_SelectedProject->GetAssetManager().FetchAssetAssuredType<Script>(id);

		if (asset)
		{
			ScriptData data = ScriptingEngine::ExtractScript(asset->GetPath());

			component.SetHandle(id);
			if(data.PyOnCreate && data.PyOnUpdate)
				component.Create(data);
		}
		
		return component;
	}

	PerspectiveCameraComponent ComponentLoader::LoadPerspectiveCamera(const json& jsonComponent)
	{
		PerspectiveCameraComponent component;
		component.data.Fov         = jsonComponent["Fov"].get<float>();
		component.data.AspectRatio = jsonComponent["Aspect Ratio"].get<float>();
		component.data.Near		   = jsonComponent["Near"].get<float>();
		component.data.Far		   = jsonComponent["Far"].get<float>();
		component.InUse			   = jsonComponent["In Use"].get<bool>();

		std::array<float, 3> position = jsonComponent["Position"].get<std::array<float, 3>>();
		std::array<float, 3> front    = jsonComponent["Front"].get<std::array<float, 3>>();
		std::array<float, 3> up		  = jsonComponent["Up"].get<std::array<float, 3>>();

		component.data.Position = { position[0], position[1], position[2] };
		component.data.Front    = { front[0],    front[1],    front[2] };
		component.data.Up       = { up[0],		 up[1],		  up[2] };
		
		component.Camera = CreateRef<PerspectiveCamera>(component.data);

		return component;
	}

	OrthographicCameraComponent ComponentLoader::LoadOrthographicCamera(const json& jsonComponent)
	{
		OrthographicCameraComponent component;
		component.data.Bottom = jsonComponent["Bottom"].get<float>();
		component.data.Top    = jsonComponent["Top"].get<float>();
		component.data.Left   = jsonComponent["Left"].get<float>();
		component.data.Right  = jsonComponent["Right"].get<float>();
		component.InUse		  = jsonComponent["In Use"].get<bool>();

		std::array<float, 3> position = jsonComponent["Position"].get<std::array<float, 3>>();
		std::array<float, 3> front	  = jsonComponent["Front"].get<std::array<float, 3>>();
		std::array<float, 3> up       = jsonComponent["Up"].get<std::array<float, 3>>();

		component.data.Position = { position[0], position[1], position[2] };
		component.data.Front    = { front[0],    front[1],    front[2] };
		component.data.Up       = { up[0],		 up[1],		  up[2] };

		component.Camera = CreateRef<OrthographicCamera>(component.data);

		return component;
	}

	void ComponentLoader::ClearCaches()
	{
		m_ModelCache.reset();
		m_ModelCache = CreateRef<ModelCache>();
	}

}