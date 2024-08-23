#pragma once

#include "Asset.h"

namespace TooGoodEngine {

	//currently just holds location to script
	//script engine and component are responsible for
	//loading the script data in for use

	class Script : public Asset
	{
	public:
		Script() = default;
		~Script() = default;

		virtual const AssetType GetAssetType() const { return AssetType::Script; }

		static Ref<Script> LoadScriptAssetFromFile(const std::filesystem::path& path);
	};

}
