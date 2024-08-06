#pragma once

#include "Core/Files/Json.h"
#include "ECS/Components/Components.h"

namespace TooGoodEngine {

	class ComponentWriter
	{
	public:
		static void WriteTransform(JsonWriter& writer, const JsonPath& entityPath, TransformComponent& component);
		static void WriteMesh(JsonWriter& writer, const JsonPath& entityPath, MeshComponent& component);
	};

}