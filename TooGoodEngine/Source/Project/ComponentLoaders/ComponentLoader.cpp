#include "ComponentLoader.h"


namespace TooGoodEngine {
		
	TransformComponent ComponentLoader::LoadTransform(const json& jsonComponent)
	{
		TransformComponent component{};

		auto& componentPosition = component.GetPosition();
		auto& componentRotation = component.GetRotation();
		auto& componentScale = component.GetScale();
		auto& componentRotationAxis = component.GetRotationAxis();

		std::vector<float> position = jsonComponent["Position"].get<std::vector<float>>();
		std::vector<float> scale = jsonComponent["Scale"].get<std::vector<float>>();
		std::vector<float> rotationAxis = jsonComponent["Rotation Axis"].get<std::vector<float>>();
		float rotation = jsonComponent["Rotation"].get<float>();

		size_t i = 0;
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
		for (; i < rotationAxis.size(); i++)
		{
			if (i >= 3) //too many
				break;

			componentRotationAxis[i] = rotationAxis[i];
		}

		componentRotation = rotation;

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

		component.PathToSource = "";
		//TODO: load the source if there is one
		
		return component;
	}

}