#include "ComponentWriter.h"

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
		path.insert(path.end(), { "Transform", "Rotation Axis" });

		std::array<float, 3> rotationAxis = { component.GetRotationAxis()[0], component.GetRotationAxis()[1], component.GetRotationAxis()[2] };
		writer.WriteGeneric(path, rotationAxis);

		path = entityPath;
		path.insert(path.end(), { "Transform", "Rotation" });

		float rotation = component.GetRotation();
		writer.WriteGeneric(path, rotation);

	}

	void ComponentWriter::WriteMesh(JsonWriter& writer, const JsonPath& entityPath, MeshComponent& component)
	{
		JsonPath path = entityPath;
		path.insert(path.end(), { "Mesh" });

		writer.WriteGeneric(path, component.PathToSource);
	}

}