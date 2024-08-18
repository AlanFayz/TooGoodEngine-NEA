#include "ComponentLoader.h"

#include "Project/Project.h"


namespace TooGoodEngine {
		
	//TODO: should take in the current project as a pointer to its paramters.

	static std::unordered_map<uint64_t, ModelInfo> s_ModelCache;

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

		component.ModelAssetId = jsonComponent.get<uint64_t>();

		if (s_ModelCache.contains(component.ModelAssetId))
		{
			component.Info = s_ModelCache[component.ModelAssetId];
			return component;
		}

		Ref<Model> model = g_SelectedProject->GetAssetManager().FetchAssetAssuredType<Model>(component.ModelAssetId);
		component.Info = sceneRenderer.AddModel(model);

		s_ModelCache[component.ModelAssetId] = component.Info;

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

		info.AlbedoFactor = jsonComponent["Albedo Factor"].get<float>();

		//
		// ---- Metallic ----
		if (jsonComponent.contains("Metallic Image"))
		{
			UUID handle = jsonComponent["Metallic Image"].get<uint64_t>();
			info.MetallicTexture = g_SelectedProject->GetAssetManager().FetchAssetAssuredType<Image>(handle);
		}

		{
			std::array<float, 4> metallicCol = jsonComponent["Metallic"].get<std::array<float, 4>>();
			info.Metallic = { metallicCol[0], metallicCol[1], metallicCol[2], metallicCol[3] };
		}

		info.MetallicFactor = jsonComponent["Metallic Factor"].get<float>();

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

		info.MetallicFactor = jsonComponent["Emission Factor"].get<float>();

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

}