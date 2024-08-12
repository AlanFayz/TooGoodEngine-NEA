#include "ComponentWriter.h"

#include <glm/gtc/type_ptr.hpp>

namespace TooGoodEngine {

	void ComponentWriter::WriteTransform(JsonWriter& writer, const JsonPath& entityPath, TransformComponent& component)
	{
		JsonPath path = entityPath;
		path.insert(path.end(), { "Transform", "Position" });

		std::array<float, 3> position = { component.GetPosition()[0], component.GetPosition()[1], component.GetPosition()[2] };
		writer.WriteGeneric(path, position);

		path = entityPath;
		path.insert(path.end(), { "Transform", "Scale" });

		std::array<float, 3> scale = { component.GetScale()[0], component.GetScale()[1], component.GetScale()[2]};
		writer.WriteGeneric(path, scale);

		path = entityPath;
		path.insert(path.end(), { "Transform", "Rotation" });

		std::array<float, 3> rotationAxis = { component.GetRotation()[0], component.GetRotation()[1], component.GetRotation()[2] };
		writer.WriteGeneric(path, rotationAxis);
	}

	void ComponentWriter::WriteMesh(JsonWriter& writer, const JsonPath& entityPath, MeshComponent& component)
	{
		JsonPath path = entityPath;
		path.insert(path.end(), { "Mesh" });

		writer.WriteGeneric(path, component.PathToSource);
	}

	void ComponentWriter::WriteMaterial(JsonWriter& writer, const JsonPath& entityPath, MaterialComponent& component)
	{
		//TODO: change from image path to UUID once asset manager is seriliazed.

		JsonPath path = entityPath;
		
		//
		// ---- Ambient ----
		if (component.Material.Ambient.ImageComponent)
		{
			path.insert(path.end(), { "Material", "Ambient Image" });
			writer.WriteGeneric(path, component.Material.Ambient.ImageComponent->GetPath().string());
		}
		 
		{
			path = entityPath;

			path.insert(path.end(), { "Material", "Ambient" });
			auto& color = component.Material.Ambient.Component;
			std::array<float, 4> attribute = { color[0], color[1], color[2], color[3] };
			writer.WriteGeneric(path, attribute);
		}

		path = entityPath;

		//
		// ---- Albedo ----
		if (component.Material.Albedo.ImageComponent)
		{
			path.insert(path.end(), { "Material", "Albedo Image" });
			writer.WriteGeneric(path, component.Material.Albedo.ImageComponent->GetPath().string());
		}
		
		{
			path = entityPath;

			path.insert(path.end(), { "Material", "Albedo" });
			auto& color = component.Material.Albedo.Component;
			std::array<float, 4> attribute = { color[0], color[1], color[2], color[3] };
			writer.WriteGeneric(path, attribute);
		}
		

		path = entityPath;
		path.insert(path.end(), { "Material", "Albedo Factor" });
		writer.WriteGeneric(path, component.Material.AlbedoFactor);

		path = entityPath;

		//
		// ---- Metallic ----
		if (component.Material.Metallic.ImageComponent)
		{
			path.insert(path.end(), { "Material", "Metallic Image" });
			writer.WriteGeneric(path, component.Material.Metallic.ImageComponent->GetPath().string());
		}

		{
			path = entityPath;

			path.insert(path.end(), { "Material", "Metallic" });
			auto& color = component.Material.Metallic.Component;
			std::array<float, 4> attribute = { color[0], color[1], color[2], color[3] };
			writer.WriteGeneric(path, attribute);
		}
	
		

		path = entityPath;
		path.insert(path.end(), { "Material", "Metallic Factor" });
		writer.WriteGeneric(path, component.Material.MetallicFactor);

		path = entityPath;

		//
		// ---- Emission ----
		if (component.Material.Emission.ImageComponent)
		{
			path.insert(path.end(), { "Material", "Emission Image" });
			writer.WriteGeneric(path, component.Material.Emission.ImageComponent->GetPath().string());
		}

		{
			path = entityPath;

			path.insert(path.end(), { "Material", "Emission" });
			auto& color = component.Material.Emission.Component;
			std::array<float, 4> attribute = { color[0], color[1], color[2], color[3] };
			writer.WriteGeneric(path, attribute);
		}

		path = entityPath;
		path.insert(path.end(), { "Material", "Emission Factor" });
		writer.WriteGeneric(path, component.Material.EmissionFactor);


		path = entityPath;

		//
		// ---- Roughness ----
		if (component.Material.Roughness.ImageComponent)
		{
			path.insert(path.end(), { "Material", "Roughness Image" });
			writer.WriteGeneric(path, component.Material.Roughness.ImageComponent->GetPath().string());
		}
		
		{
			path = entityPath;

			path.insert(path.end(), { "Material", "Roughness" });
			auto& color = component.Material.Roughness.Component;
			writer.WriteGeneric(path, color[0]);
		}
		
	}

	void ComponentWriter::WritePointLight(JsonWriter& writer, const JsonPath& entityPath, PointLightComponent& component)
	{
		JsonPath path = entityPath;

		{
			path.insert(path.end(), { "Point Light", "Color" });
			std::array<float, 4> color = { component.Color[0], component.Color[1], component.Color[2], component.Color[3] };
			writer.WriteGeneric(path, color);
		}

		path = entityPath;

		{
			path.insert(path.end(), { "Point Light", "Position" });
			std::array<float, 3> position = { component.Position[0], component.Position[1], component.Position[2] };
			writer.WriteGeneric(path, position);
		}
		
		path = entityPath;

		{
			path.insert(path.end(), { "Point Light", "Radius" });
			writer.WriteGeneric(path, component.Radius);
		}

		path = entityPath;

		{
			path.insert(path.end(), { "Point Light", "Intensity" });
			writer.WriteGeneric(path, component.Intensity);
		}
	}

	void ComponentWriter::WriteDirectionalLight(JsonWriter& writer, const JsonPath& entityPath, DirectionalLightComponent& component)
	{
		JsonPath path = entityPath;

		{
			path.insert(path.end(), { "Directional Light", "Color" });
			std::array<float, 4> color = { component.Color[0], component.Color[1], component.Color[2], component.Color[3] };
			writer.WriteGeneric(path, color);
		}

		path = entityPath;

		{
			path.insert(path.end(), { "Directional Light", "Direction" });
			std::array<float, 3> direction = { component.Direction[0], component.Direction[1], component.Direction[2] };
			writer.WriteGeneric(path, direction);
		}

		path = entityPath;

		{
			path.insert(path.end(), { "Directional Light", "Intensity" });
			writer.WriteGeneric(path, component.Intensity);
		}
		
	}

}