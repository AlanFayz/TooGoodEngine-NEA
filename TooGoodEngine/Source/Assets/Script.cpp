#include "Script.h"

namespace TooGoodEngine {
	Ref<Script> Script::LoadScriptAssetFromFile(const std::filesystem::path& path)
	{
		return CreateRef<Script>();
	}
}