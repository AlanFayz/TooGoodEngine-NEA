#pragma once

#include "Alignment.h"
#include "API/OpenGL/Texture2D.h"
#include "Assets/Image.h"

#include <inttypes.h>
#include <glm/glm.hpp>

namespace TooGoodEngine {

	enum class MaterialType
	{
		None = 0, Image, Vector
	};

	struct MaterialAttribute
	{
		glm::vec4 Component;
		Ref<Image> ImageComponent = nullptr; //simply here to keep a lifetime.
		uint64_t  BindlessTextureHandle = 0; //uvec2 in the shader
		MaterialType Type;
	};

	struct Material
	{
		MaterialAttribute Ambient;
		MaterialAttribute Albedo;
		MaterialAttribute Metallic;
		MaterialAttribute Emission;
		MaterialAttribute Roughness;

		float AlbedoFactor;  
		float MetallicFactor;
		float EmissionFactor;
 
		void MakeHandlesResident() const;
		void MakeHandlesNonResident() const;
		void UpdateTypes();
	};

	struct MaterialInfo
	{
		glm::vec4 Ambient;
		Ref<Image> AmbientTexture = nullptr;

		glm::vec4 Albedo;
		Ref<Image> AlbedoTexture = nullptr;
		float AlbedoFactor;

		glm::vec4 Metallic;
		Ref<Image> MetallicTexture = nullptr;
		float MetallicFactor;

		glm::vec4 Emission;
		Ref<Image> EmissionTexture = nullptr;
		float EmissionFactor;

		float Roughness;
		Ref<Image> RoughnessTexture = nullptr;
	};

	Material CreateMaterial(const MaterialInfo& info);

}