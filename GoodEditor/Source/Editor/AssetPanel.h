#pragma once

#include "Project/Project.h"

#include <imgui.h>
#include <set>

namespace GoodEditor {

	using namespace TooGoodEngine;

	class AssetPanel
	{
	public:
		static void DrawAssetPanel(std::map<std::filesystem::path, Ref<Image>>& extensionMap);

	private:
		static std::filesystem::path m_CurrentDirectory;
		static std::set<std::filesystem::path> m_CachedDirectories;
	};

}