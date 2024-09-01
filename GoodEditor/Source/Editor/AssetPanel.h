#pragma once

#include <Project/Project.h>
#include <imgui.h>
#include <set>
#include <functional>


namespace GoodEditor {

	using namespace TooGoodEngine;


	class AssetPanel
	{
	public:
		using ExtensionAssetLoadMap = std::map<std::filesystem::path, std::function<Ref<Asset>(const std::filesystem::path&)>>;

	public:
		static void DrawPanel(std::map<std::filesystem::path, Ref<Image>>& extensionMap);

	private:
		static inline std::filesystem::path s_CurrentDirectory;
		static inline std::set<std::filesystem::path> s_CachedDirectories;

		//init in the cpp
		static const ExtensionAssetLoadMap s_ExtensionFunctions;
	};

}