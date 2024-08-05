#pragma once

#include "Alignment.h"

#include <inttypes.h>
#include <glm/glm.hpp>

namespace TooGoodEngine {

	enum class MaterialType
	{
		None = 0, Image, Vector
	};

	struct MaterialComponent
	{
		TGE_SCALAR_ALIGNMENT MaterialType Type;

		union TGE_VECTOR_4_ALIGNMENT
		{
			glm::vec4 Component;
			uint32_t  ImageIndex;
		};
	};

	struct Material
	{
		MaterialComponent Albedo;
		MaterialComponent Metallic;
		MaterialComponent Emission;
		MaterialComponent Roughness;

		TGE_SCALAR_ALIGNMENT float AlbedoFactor;  
		TGE_SCALAR_ALIGNMENT float MetallicFactor;
		TGE_SCALAR_ALIGNMENT float EmissionFactor;
		TGE_SCALAR_ALIGNMENT float RoughnessFactor;
	};

}