#pragma once

#include "Core/UUID.h"

#include <filesystem>

namespace TooGoodEngine {
	
	enum class AssetType
	{
		None = 0, //more to add
		Texture,
		Model,
		Scene
	};

	class Asset
	{
	public:
		Asset() = default;
		Asset(UUID other) : m_UUID(other) {}

		void SetID(UUID other) { m_UUID = other; }

		virtual ~Asset() = default;
		virtual const AssetType GetAssetType() = 0;
		 
		inline const UUID GetAssetID() const { return m_UUID; }

		//template specilization 
		template<typename T>
		static Ref<T> LoadAssetFromFile(const std::filesystem::path& path, bool isBinary)
		{
			TGE_LOG_ERROR("current asset type not supported ", typeid(T).name());
			return nullptr;
		}

	private:
		UUID m_UUID;
	};


}