#pragma once

#include "Core/Base.h"
#include "Asset.h"

#include <filesystem>
#include <map>

namespace TooGoodEngine {

	using AssetBank = std::map<UUID, Ref<Asset>>;

	class AssetManager
	{
	public:
		AssetManager() = default;
		~AssetManager() = default;

		template<typename T>
		Ref<T> GetAsset(UUID uuid)
		{
			return std::dynamic_pointer_cast<T>(FetchAsset(uuid));
		}

		template<typename T>
		Ref<T> FetchAndLoadAssetIntoBank(const std::filesystem::path& path, bool isBinary)
		{
			static_assert(std::is_base_of_v<Asset, T>, "not a valid asset");
			Ref<T> asset = Asset::LoadAssetFromFile<T>(path, isBinary);
			
			if (!asset)
			{
				TGE_LOG_WARNING("Failed to load asset of type ", typeid(T).name(), " path ", path);
				return asset;
			}

			UUID uuid = asset->GetAssetID();

			m_AssetBank[uuid] = asset;
			return asset;
		}

		Ref<Asset> FetchAsset(UUID uuid);
		void	   RemoveAsset(UUID uuid);
			
	private:
		AssetBank m_AssetBank;
	};

}