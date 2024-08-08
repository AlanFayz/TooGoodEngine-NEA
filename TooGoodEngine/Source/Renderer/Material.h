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

	struct MaterialComponent
	{
		glm::vec4 Component;
		uint64_t  BindlessTextureHandle = 0; //uvec2 in the shader
		MaterialType Type;
	};

	//NOTE: currently always assumes the texture is valid, is this ok??
	struct Material
	{
		MaterialComponent Albedo;
		MaterialComponent Metallic;
		MaterialComponent Emission;
		MaterialComponent Roughness;

		float AlbedoFactor;  
		float MetallicFactor;
		float EmissionFactor;
 
		void MakeHandlesResident() const;
		void MakeHandlesNonResident() const;
	};

	struct MaterialInfo
	{
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