#pragma once

#include "Project/Project.h"

#include <imgui.h>


namespace GoodEditor {

	using namespace TooGoodEngine;

	/*
		Simply displays the currently loaded assets
	*/

	class AssetPanel
	{
	public:
		static void DrawAssets();

	private:
		static void _ShowAssetPannelPopup();

	};

}