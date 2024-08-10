#include "Material.h"

namespace TooGoodEngine {
	Material CreateMaterial(const MaterialInfo& info)
	{
		Material material{};

		if (info.AmbientTexture)
		{
			material.Ambient.Type = MaterialType::Image;
			material.Ambient.BindlessTextureHandle = info.AmbientTexture->GetTexture().GetAddress();
			material.Ambient.ImageComponent = info.AmbientTexture;
		}
		else
		{
			material.Ambient.Type = MaterialType::Vector;
			material.Ambient.Component = info.Ambient;
		}

		if (info.AlbedoTexture)
		{
			material.Albedo.Type = MaterialType::Image;
			material.Albedo.BindlessTextureHandle = info.AlbedoTexture->GetTexture().GetAddress();
			material.Albedo.ImageComponent = info.AlbedoTexture;

		}
		else
		{
			material.Albedo.Type = MaterialType::Vector;
			material.Albedo.Component = info.Albedo;
		}

		material.AlbedoFactor = info.AlbedoFactor;

		if (info.MetallicTexture)
		{
			material.Metallic.Type = MaterialType::Image;
			material.Metallic.BindlessTextureHandle = info.MetallicTexture->GetTexture().GetAddress();
			material.Metallic.ImageComponent = info.MetallicTexture;
		}
		else
		{
			material.Metallic.Type = MaterialType::Vector;
			material.Metallic.Component = info.Metallic;
		}

		material.MetallicFactor = info.MetallicFactor;

		if (info.EmissionTexture)
		{
			material.Emission.Type = MaterialType::Image;
			material.Emission.BindlessTextureHandle = info.EmissionTexture->GetTexture().GetAddress();
			material.Emission.ImageComponent = info.EmissionTexture;
		}
		else
		{
			material.Emission.Type = MaterialType::Vector;
			material.Emission.Component = info.Emission;
		}

		material.EmissionFactor = info.EmissionFactor;

		if (info.RoughnessTexture)
		{
			material.Roughness.Type = MaterialType::Image;
			material.Roughness.BindlessTextureHandle = info.RoughnessTexture->GetTexture().GetAddress();
			material.Roughness.ImageComponent = info.RoughnessTexture;
		}
		else
		{
			material.Roughness.Type = MaterialType::Vector;
			material.Roughness.Component = glm::vec4(info.Roughness, 0.0f, 0.0f, 0.0f);
		}

		return material;
	}
	void Material::MakeHandlesResident() const
	{
		if (Ambient.Type == MaterialType::Image)
			Ambient.ImageComponent->GetTexture().MakeResident();

		if (Albedo.Type == MaterialType::Image)
			Albedo.ImageComponent->GetTexture().MakeResident();

		if (Metallic.Type == MaterialType::Image)
			Metallic.ImageComponent->GetTexture().MakeResident();

		if (Emission.Type == MaterialType::Image)
			Emission.ImageComponent->GetTexture().MakeResident();

		if (Roughness.Type == MaterialType::Image)
			Roughness.ImageComponent->GetTexture().MakeResident();

	}
	void Material::MakeHandlesNonResident() const
	{
		if (Ambient.Type == MaterialType::Image)
			Ambient.ImageComponent->GetTexture().MakeNonResident();

		if (Albedo.Type  == MaterialType::Image)
			Albedo.ImageComponent->GetTexture().MakeNonResident();

		if (Metallic.Type == MaterialType::Image)
			Metallic.ImageComponent->GetTexture().MakeNonResident();

		if (Emission.Type == MaterialType::Image)
			Emission.ImageComponent->GetTexture().MakeNonResident();

		if (Roughness.Type == MaterialType::Image)
			Roughness.ImageComponent->GetTexture().MakeNonResident();
	}
}