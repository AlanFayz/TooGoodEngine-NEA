#include "AssetManager.h"
#include "Project/Project.h"

namespace TooGoodEngine {
	AssetManager::AssetManager(const std::filesystem::path& assetDirectory)
		: m_AssetDirectory(assetDirectory)
	{
	}
	Ref<Asset> AssetManager::FetchAsset(const std::filesystem::path& path)
	{
		std::filesystem::path relative = std::filesystem::relative(path, GetPath());
		if(m_AssetCache.contains(relative))
			return FetchAsset(m_AssetCache[relative]);

		return nullptr;
	}
	Ref<Asset> AssetManager::FetchAsset(UUID uuid)
	{
		return m_AssetBank.contains(uuid) ? m_AssetBank[uuid] : nullptr;
	}
	void AssetManager::RemoveAsset(UUID uuid)
	{
		if (m_AssetBank.contains(uuid))
			m_AssetBank.erase(uuid);
	}
	void AssetManager::RemoveAsset(const std::filesystem::path& path)
	{
		std::filesystem::path relative = std::filesystem::relative(path, GetPath());
		
		if (m_AssetCache.contains(relative))
		{
			RemoveAsset(m_AssetCache[relative]);
			m_AssetCache.erase(relative);
		}
	}

}
