#pragma once

#include "Core/Files/Json.h"
#include "ECS/Components/Components.h"

namespace TooGoodEngine {

	class ComponentWriter
	{
	public:
		static void WriteTransform(JsonWriter& writer, const JsonPath& entityPath, TransformComponent& component);
		static void WriteMesh(JsonWriter& writer, const JsonPath& entityPath, MeshComponent& component);
		static void WriteModel(JsonWriter& writer, const JsonPath& entityPath, ModelComponent& component);
		static void WriteMaterial(JsonWriter& writer, const JsonPath& entityPath, MaterialComponent& component);
		static void WritePointLight(JsonWriter& writer, const JsonPath& entityPath, PointLightComponent& component);
		static void WriteDirectionalLight(JsonWriter& writer, const JsonPath& entityPath, DirectionalLightComponent& component);
		static void WriteScript(JsonWriter& writer, const JsonPath& entityPath, ScriptComponent& component);
		static void WritePerspectiveCamera(JsonWriter& writer, const JsonPath& entityPath, PerspectiveCameraComponent& component);
		static void WriteOrthographicCamera(JsonWriter& writer, const JsonPath& entityPath, OrthographicCameraComponent& component);
	};

}