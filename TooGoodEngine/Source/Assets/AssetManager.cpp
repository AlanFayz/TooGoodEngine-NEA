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
}
