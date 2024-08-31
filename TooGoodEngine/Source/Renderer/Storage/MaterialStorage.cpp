#include "MaterialStorage.h"

namespace TooGoodEngine {

	size_t MaterialStorage::Create()
	{
		size_t index = 0;

		if (!m_AvailableSlots.empty())
		{
			index = m_AvailableSlots.top();
			m_AvailableSlots.pop();
			m_Storage.Emplace(index);

			m_MetaData[index] = {};
			return index;
		}
		
		index = m_MetaData.size();
		m_Storage.Emplace(index);

		m_MetaData.push_back({});

		return index;
	}

	size_t MaterialStorage::Create(const MaterialInfo& info)
	{
		size_t index = 0;

		if (!m_AvailableSlots.empty())
		{
			index = m_AvailableSlots.top();
			m_AvailableSlots.pop();
		}
		else
		{
			index = m_MetaData.size();
			m_MetaData.push_back(info);
		}

		Material material = _Create(info);
		m_Storage.Insert(index, material);

		return index;
	}

	size_t MaterialStorage::GetIndex(size_t index)
	{
		return m_Storage.GetSparse()[index];
	}

	MaterialInfo& MaterialStorage::GetInfo(size_t index)
	{
		return m_MetaData[index];
	}

	void MaterialStorage::Modify(size_t index, const MaterialInfo& info)
	{
		if (!m_Storage.Contains(index))
		{
			TGE_LOG_WARNING("failed to modify material ", index);
			return;
		}

		Material& material     = m_Storage.Get(index);
		MaterialInfo& metaData = m_MetaData[index];

		if(metaData.AmbientTexture)
			metaData.AmbientTexture->GetTexture().MakeNonResident();
		
		if(metaData.AlbedoTexture)
			metaData.AlbedoTexture->GetTexture().MakeNonResident();
		
		if(metaData.MetallicTexture)
			metaData.MetallicTexture->GetTexture().MakeNonResident();
		
		if(metaData.EmissionTexture)
			metaData.EmissionTexture->GetTexture().MakeNonResident();
		
		if(metaData.RoughnessTexture)
			metaData.RoughnessTexture->GetTexture().MakeNonResident();

		material = _Create(info);
		metaData = info;	
	}

	void MaterialStorage::Remove(size_t index)
	{
		m_AvailableSlots.push(index);
		m_Storage.Remove(index);
		m_MetaData[index] = {}; //clear the meta data as well as it contians refs
	}

	void MaterialStorage::SubmitBuffer(uint32_t index) const
	{
		auto first = m_Storage.GetDense().data();
		m_Storage.GetDenseAllocator().SubmitBuffer(first, index);
	}

	Material MaterialStorage::_Create(const MaterialInfo& info)
	{
		Material material{};

		if (info.AmbientTexture)
		{
			info.AmbientTexture->GetTexture().MakeResident();
			material.Ambient.BindlessTextureHandle = info.AmbientTexture->GetTexture().GetAddress();
		}

		material.Ambient.Component = info.Ambient;

		if (info.AlbedoTexture)
		{
			info.AlbedoTexture->GetTexture().MakeResident();
			material.Albedo.BindlessTextureHandle = info.AlbedoTexture->GetTexture().GetAddress();
		}

		material.Albedo.Component = info.Albedo;

		if (info.MetallicTexture)
		{
			info.MetallicTexture->GetTexture().MakeResident();
			material.Metallic.BindlessTextureHandle = info.MetallicTexture->GetTexture().GetAddress();
		}

		material.Metallic.Component.r = info.Metallic;

		if (info.EmissionTexture)
		{
			info.EmissionTexture->GetTexture().MakeResident();
			material.Emission.BindlessTextureHandle = info.EmissionTexture->GetTexture().GetAddress();
		}


		material.Emission.Component = info.Emission;
		material.EmissionFactor = info.EmissionFactor;

		if (info.RoughnessTexture)
		{
			info.RoughnessTexture->GetTexture().MakeResident();
			material.Roughness.BindlessTextureHandle = info.RoughnessTexture->GetTexture().GetAddress();
		}

		material.Roughness.Component = glm::vec4(info.Roughness, 0.0f, 0.0f, 0.0f);

		return material;
	}

}