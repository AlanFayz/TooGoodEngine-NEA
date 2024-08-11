#include "AssetManager.h"

namespace TooGoodEngine {

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
		std::filesystem::path absolute = std::filesystem::absolute(path);
		if (m_AssetCache.contains(absolute))
			RemoveAsset(m_AssetCache[absolute]);
	}
}
