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
		AssetManager(const std::filesystem::path& assetDirectory);
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

			std::filesystem::path relative = std::filesystem::relative(path, GetPath());

			if (m_AssetCache.contains(relative))
				return FetchAssetAssuredType<T>(m_AssetCache[relative]);

			T type;
			Ref<Asset> asset = Asset::LoadAssetFromFile(type.GetAssetType(), path);
			
			if (!asset)
			{
				TGE_LOG_WARNING("Failed to load asset of type ", typeid(T).name(), " path ", path);
				return nullptr;
			}

			UUID uuid = asset->GetAssetID();

			m_AssetCache[relative] = uuid;
			m_AssetBank[uuid] = asset;
			return dynamic_pointer_cast<T>(asset);
		}

		template<typename T>
		Ref<T> FetchAndLoadAssetWithID(const std::filesystem::path& path, UUID id)
		{
			static_assert(std::is_base_of_v<Asset, T>, "not a valid asset");

			std::filesystem::path relative = std::filesystem::relative(path, GetPath());

			if (m_AssetCache.contains(relative))
				return FetchAssetAssuredType<T>(m_AssetCache[relative]);

			T type;
			Ref<Asset> asset = Asset::LoadAssetFromFile(type.GetAssetType(), path);

			if (!asset)
			{
				TGE_LOG_WARNING("Failed to load asset of type ", typeid(T).name(), " path ", path);
				return dynamic_pointer_cast<T>(asset);
			}

			asset->SetID(id);
			m_AssetCache[relative] = id;
			m_AssetBank[id] = asset;
			return dynamic_pointer_cast<T>(asset);
		}

		Ref<Asset> FetchAsset(const std::filesystem::path& path);
		Ref<Asset> FetchAsset(UUID uuid);
		void	   RemoveAsset(UUID uuid);
		void       RemoveAsset(const std::filesystem::path& path);
		
		inline const std::filesystem::path& GetPath() const { return m_AssetDirectory; }
		
		const AssetBank& GetBank() const { return m_AssetBank; } //simply for rendering

	private:
		AssetBank m_AssetBank{};
		AssetCache m_AssetCache;

		std::filesystem::path m_AssetDirectory;
	};

}