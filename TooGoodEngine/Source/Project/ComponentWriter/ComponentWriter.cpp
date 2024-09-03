#include "ComponentWriter.h"

#include <glm/gtc/type_ptr.hpp>

namespace TooGoodEngine {

	void ComponentWriter::WriteTransform(JsonWriter& writer, const JsonPath& entityPath, TransformComponent& component)
	{
		JsonPath path = entityPath;
		path.insert(path.end(), { "Transform", "Position" });

		std::array<float, 3> position = { component.Position[0], component.Position[1], component.Position[2] };
		writer.WriteGeneric(path, position);

		path = entityPath;
		path.insert(path.end(), { "Transform", "Scale" });

		std::array<float, 3> scale = { component.Scale[0], component.Scale[1], component.Scale[2]};
		writer.WriteGeneric(path, scale);

		path = entityPath;
		path.insert(path.end(), { "Transform", "Rotation" });

		std::array<float, 3> rotationAxis = { component.Rotation[0], component.Rotation[1], component.Rotation[2] };
		writer.WriteGeneric(path, rotationAxis);
	}

	void ComponentWriter::WriteMesh(JsonWriter& writer, const JsonPath& entityPath, MeshComponent& component)
	{
		JsonPath path = entityPath;
		path.insert(path.end(), { "Mesh" });

		writer.WriteGeneric(path, component.PathToSource);
	}

	void ComponentWriter::WriteModel(JsonWriter& writer, const JsonPath& entityPath, ModelComponent& component)
	{
		JsonPath path = entityPath;
		path.insert(path.end(), { "Model" });

		writer.WriteGeneric(path, (uint64_t)component.ModelAssetId);
	}

	void ComponentWriter::WriteMaterial(JsonWriter& writer, const JsonPath& entityPath, MaterialComponent& component)
	{
		JsonPath path = entityPath;

		auto& materialInfo = component.Renderer->GetMaterialInfo(component.ID);
		
		//
		// ---- Ambient ----
		//
		if (materialInfo.AmbientTexture)
		{
			path.insert(path.end(), { "Material", "Ambient Image" });
			writer.WriteGeneric(path, (uint64_t)materialInfo.AmbientTexture->GetAssetID());
		}
		 
		{
			path = entityPath;

			path.insert(path.end(), { "Material", "Ambient" });
			auto& color = materialInfo.Ambient;
			std::array<float, 4> attribute = { color[0], color[1], color[2], color[3] };
			writer.WriteGeneric(path, attribute);
		}

		path = entityPath;

		//
		// ---- Albedo ----
		//
		if (materialInfo.AlbedoTexture)
		{
			path.insert(path.end(), { "Material", "Albedo Image" });
			writer.WriteGeneric(path, (uint64_t)materialInfo.AlbedoTexture->GetAssetID());
		}
		
		{
			path = entityPath;

			path.insert(path.end(), { "Material", "Albedo" });
			auto& color = materialInfo.Albedo;
			std::array<float, 4> attribute = { color[0], color[1], color[2], color[3] };
			writer.WriteGeneric(path, attribute);
		}

		path = entityPath;

		//
		// ---- Metallic ----
		//
		if (materialInfo.MetallicTexture)
		{
			path.insert(path.end(), { "Material", "Metallic Image" });
			writer.WriteGeneric(path, (uint64_t)materialInfo.MetallicTexture->GetAssetID());
		}

		{
			path = entityPath;

			path.insert(path.end(), { "Material", "Metallic" });
			auto& color = materialInfo.Metallic;
			writer.WriteGeneric(path, color);
		}
	
		
		path = entityPath;

		//
		// ---- Emission ----
		//
		if (materialInfo.EmissionTexture)
		{
			path.insert(path.end(), { "Material", "Emission Image" });
			writer.WriteGeneric(path, (uint64_t)materialInfo.EmissionTexture->GetAssetID());
		}

		{
			path = entityPath;

			path.insert(path.end(), { "Material", "Emission" });
			auto& color = materialInfo.Emission;
			std::array<float, 4> attribute = { color[0], color[1], color[2], color[3] };
			writer.WriteGeneric(path, attribute);
		}

		path = entityPath;
		path.insert(path.end(), { "Material", "Emission Factor" });
		writer.WriteGeneric(path, materialInfo.EmissionFactor);


		path = entityPath;

		//
		// ---- Roughness ----
		//
		if (materialInfo.RoughnessTexture)
		{
			path.insert(path.end(), { "Material", "Roughness Image" });
			writer.WriteGeneric(path, (uint64_t)materialInfo.RoughnessTexture->GetAssetID());
		}
		
		{
			path = entityPath;

			path.insert(path.end(), { "Material", "Roughness" });
			auto& color = materialInfo.Roughness;
			writer.WriteGeneric(path, color);
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

	void ComponentWriter::WriteScript(JsonWriter& writer, const JsonPath& entityPath, ScriptComponent& component)
	{
		JsonPath path = entityPath;

		path.insert(path.end(), { "Script" });
		writer.WriteGeneric(path, (uint64_t)component.GetHandle());
	}

	void ComponentWriter::WritePerspectiveCamera(JsonWriter& writer, const JsonPath& entityPath, PerspectiveCameraComponent& component)
	{
		JsonPath path = entityPath;

		{
			path.insert(path.end(), { "Perspective Camera", "Fov"});
			writer.WriteGeneric(path, component.data.Fov);
		}

		path = entityPath;

		{
			path.insert(path.end(), { "Perspective Camera", "Aspect Ratio" });
			writer.WriteGeneric(path, component.data.AspectRatio);
		}


		path = entityPath;

		{
			path.insert(path.end(), { "Perspective Camera", "Near" });
			writer.WriteGeneric(path, component.data.Near);
		}

		path = entityPath;

		{
			path.insert(path.end(), { "Perspective Camera", "Far" });
			writer.WriteGeneric(path, component.data.Far);
		}

		path = entityPath;

		{
			path.insert(path.end(), { "Perspective Camera", "Position" });

			std::array<float, 3> position = { component.data.Position.x, component.data.Position.y, component.data.Position.z };
			writer.WriteGeneric(path, position);
		}

		path = entityPath;

		{
			path.insert(path.end(), { "Perspective Camera", "Front" });

			std::array<float, 3> front = { component.data.Front.x, component.data.Front.y, component.data.Front.z };
			writer.WriteGeneric(path, front);
		}

		path = entityPath;

		{
			path.insert(path.end(), { "Perspective Camera", "Up" });

			std::array<float, 3> up = { component.data.Up.x, component.data.Up.y, component.data.Up.z };
			writer.WriteGeneric(path, up);
		}

		path = entityPath;

		{
			path.insert(path.end(), { "Perspective Camera", "In Use" });
			writer.WriteGeneric(path, component.InUse);
		}
	}

	void ComponentWriter::WriteOrthographicCamera(JsonWriter& writer, const JsonPath& entityPath, OrthographicCameraComponent& component)
	{
		JsonPath path = entityPath;

		{
			path.insert(path.end(), { "Orthographic Camera", "Bottom" });
			writer.WriteGeneric(path, component.data.Bottom);
		}

		path = entityPath;

		{
			path.insert(path.end(), { "Orthographic Camera", "Top" });
			writer.WriteGeneric(path, component.data.Top);
		}


		path = entityPath;

		{
			path.insert(path.end(), { "Orthographic Camera", "Left" });
			writer.WriteGeneric(path, component.data.Left);
		}

		path = entityPath;

		{
			path.insert(path.end(), { "Orthographic Camera", "Right" });
			writer.WriteGeneric(path, component.data.Right);
		}

		path = entityPath;

		{
			path.insert(path.end(), { "Orthographic Camera", "Position" });

			std::array<float, 3> position = { component.data.Position.x, component.data.Position.y, component.data.Position.z };
			writer.WriteGeneric(path, position);
		}

		path = entityPath;

		{
			path.insert(path.end(), { "Orthographic Camera", "Front" });

			std::array<float, 3> front = { component.data.Front.x, component.data.Front.y, component.data.Front.z };
			writer.WriteGeneric(path, front);
		}

		path = entityPath;

		{
			path.insert(path.end(), { "Orthographic Camera", "Up" });

			std::array<float, 3> up = { component.data.Up.x, component.data.Up.y, component.data.Up.z };
			writer.WriteGeneric(path, up);
		}

		path = entityPath;

		{
			path.insert(path.end(), { "Orthographic Camera", "In Use" });
			writer.WriteGeneric(path, component.InUse);
		}
	}

}