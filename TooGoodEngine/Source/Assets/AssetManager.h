#pragma once

#include "Core/Base.h"
#include "Asset.h"

#include <filesystem>
#include <map>

namespace TooGoodEngine {

	using AssetBank  = std::map<UUID, Ref<Asset>>;
	using AssetCache = std::map<std::filesystem::path, UUID>;

	class AssetManager
	{
	public:
		AssetManager() = default;
		~AssetManager() = default;

		template<typename T>
		Ref<T> FetchAssetAssuredType(UUID uuid)
		{
			return std::dynamic_pointer_cast<T>(FetchAsset(uuid));
		}

		template<typename T>
		Ref<T> LoadAssetIntoBank(const std::filesystem::path& path)
		{
			static_assert(std::is_base_of_v<Asset, T>, "not a valid asset");

			std::filesystem::path absolute = std::filesystem::absolute(path);

			if (m_AssetCache.contains(absolute))
				return FetchAssetAssuredType<T>(m_AssetCache[absolute]);

			T type;
			Ref<Asset> asset = Asset::LoadAssetFromFile(type.GetAssetType(), path);
			
			if (!asset)
			{
				TGE_LOG_WARNING("Failed to load asset of type ", typeid(T).name(), " path ", path);
				return nullptr;
			}

			UUID uuid = asset->GetAssetID();

			m_AssetCache[absolute] = uuid;
			m_AssetBank[uuid] = asset;
			return dynamic_pointer_cast<T>(asset);
		}

		template<typename T>
		Ref<T> FetchAndLoadAssetWithID(const std::filesystem::path& path, UUID id, bool isBinary)
		{
			static_assert(std::is_base_of_v<Asset, T>, "not a valid asset");
			TGE_VERIFY(!m_AssetCache.contains(path), "asset has already been loaded");

			Ref<T> asset = Asset::LoadAssetFromFile(path, isBinary);

			if (!asset)
			{
				TGE_LOG_WARNING("Failed to load asset of type ", typeid(T).name(), " path ", path);
				return asset;
			}

			asset->SetID(id);
			m_AssetCache[std::filesystem::absolute(path)] = id;
			m_AssetBank[id] = asset;
			return asset;
		}

		Ref<Asset> FetchAsset(const std::filesystem::path& path);
		Ref<Asset> FetchAsset(UUID uuid);
		void	   RemoveAsset(UUID uuid);
		void       RemoveAsset(const std::filesystem::path& path);
		
		const AssetBank& GetBank() const { return m_AssetBank; } //simply for rendering

	private:
		AssetBank m_AssetBank{};
		AssetCache m_AssetCache;
	};

}