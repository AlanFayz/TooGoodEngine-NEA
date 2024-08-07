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
		TGE_SCALAR_ALIGNMENT   MaterialType Type;
		TGE_VECTOR_4_ALIGNMENT glm::vec4 Component;
		TGE_VECTOR_2_ALIGNMENT uint64_t  BindlessTextureHandle; 
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
 
		void MakeHandlesResident() const
		{ 
			if(Albedo.Type == MaterialType::Image)
				glMakeTextureHandleResidentARB((GLuint64)Albedo.BindlessTextureHandle);

			if (Metallic.Type == MaterialType::Image)
				glMakeTextureHandleResidentARB((GLuint64)Metallic.BindlessTextureHandle);

			if (Emission.Type == MaterialType::Image)
				glMakeTextureHandleResidentARB((GLuint64)Emission.BindlessTextureHandle);

			if(Roughness.Type == MaterialType::Image)
				glMakeTextureHandleResidentARB((GLuint64)Roughness.BindlessTextureHandle);

		}

		void MakeHandlesNonResident() const
		{
			if (Albedo.Type == MaterialType::Image)
				glMakeTextureHandleNonResidentARB((GLuint64)Albedo.BindlessTextureHandle);

			if (Metallic.Type == MaterialType::Image)
				glMakeTextureHandleNonResidentARB((GLuint64)Metallic.BindlessTextureHandle);

			if (Emission.Type == MaterialType::Image)
				glMakeTextureHandleNonResidentARB((GLuint64)Emission.BindlessTextureHandle);

			if (Roughness.Type == MaterialType::Image)
				glMakeTextureHandleNonResidentARB((GLuint64)Roughness.BindlessTextureHandle);
		}
	};

}