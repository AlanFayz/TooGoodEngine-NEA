#pragma once

#include "Core/SparseSet.h"
#include "Assets/Image.h"
#include "StorageBufferAllocator.h"

#include <glm/glm.hpp>
#include <stack>

namespace TooGoodEngine {


	struct MaterialInfo
	{
		glm::vec4 Ambient;
		Ref<Image> AmbientTexture = nullptr;

		glm::vec4 Albedo;
		Ref<Image> AlbedoTexture = nullptr;

		glm::vec4 Emission;
		Ref<Image> EmissionTexture = nullptr;
		float EmissionFactor;

		float Metallic;
		Ref<Image> MetallicTexture = nullptr;

		float Roughness;
		Ref<Image> RoughnessTexture = nullptr;
	};

	struct MaterialAttribute
	{
		glm::vec4 Component;
		uint64_t BindlessTextureHandle = 0;
		glm::uvec2 NotUsed;
	};

	struct Material
	{
		MaterialAttribute Ambient;
		MaterialAttribute Albedo;
		MaterialAttribute Metallic;
		MaterialAttribute Emission;
		MaterialAttribute Roughness;

		float EmissionFactor;
		float Padding;
		//size_t index (uvec2) because everything is stored as a pair in sparse set
	};

	class MaterialStorage
	{
	public:
		MaterialStorage() = default;
		~MaterialStorage() = default;

		size_t Create();
		size_t Create(const MaterialInfo& info);

		size_t GetIndex(size_t index);

		MaterialInfo& GetInfo(size_t index);

		void Modify(size_t index, const MaterialInfo& info);
		void Remove(size_t index);

		void SubmitBuffer(uint32_t index) const;

	private:
		Material _Create(const MaterialInfo& info);

	private:
		SparseSet<Material, std::numeric_limits<size_t>::max(), StorageBufferAllocator<Material>> m_Storage;
		std::vector<MaterialInfo> m_MetaData;
		std::stack<size_t> m_AvailableSlots;
	};



}