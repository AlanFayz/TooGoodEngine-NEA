#include "Application.h"
#include "Math/Random.h"
#include "Assets/AssetManager.h"
#include "Files/FileReader.h"

#include <vector>
#include <algorithm>

namespace TooGoodEngine {

	

	void Application::Run()
	{
		AssetManager manager;

		Ref<TestAsset> asset = manager.FetchAndLoadAssetIntoBank<TestAsset>("testAsset.txt", false);

		std::cout << asset->GetData() << " " << asset->GetAssetID() << std::endl;

		asset = manager.GetAsset<TestAsset>(asset->GetAssetID());

		std::cout << asset->GetData() << " " << asset->GetAssetID() << std::endl;

		manager.RemoveAsset(asset->GetAssetID());

		UUID id = asset->GetAssetID();

		asset.reset();

		asset = manager.GetAsset<TestAsset>(id);
		TGE_ASSERT(asset == nullptr, "why are you not deleted");

	}
}