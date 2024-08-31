#pragma once

#include <Project/Project.h>
#include <imgui.h>
#include <set>

namespace GoodEditor {

	using namespace TooGoodEngine;

	class AssetPanel
	{
	public:
		static void DrawPanel(std::map<std::filesystem::path, Ref<Image>>& extensionMap);

	private:
		static std::filesystem::path m_CurrentDirectory;
		static std::set<std::filesystem::path> m_CachedDirectories;
		static std::unordered_map<TooGoodEngine::UUID, ModelInfo> m_ModelInstanceCache;
	};

}