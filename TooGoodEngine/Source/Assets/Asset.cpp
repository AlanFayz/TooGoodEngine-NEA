#include "Asset.h"

#include <map>
#include <functional>

#include "Image.h"
#include "Model.h"

namespace TooGoodEngine {

	static Ref<Asset> LoadUntypedAsset(const std::filesystem::path& path) //example use case
	{
		return nullptr;
	}

	using FunctionMap = std::map<AssetType, std::function<Ref<Asset>(const std::filesystem::path&)>>;
	
	static const FunctionMap s_FunctionMap
	{
		{AssetType::None,  LoadUntypedAsset}, 
		{AssetType::Image, Image::LoadImageAssetFromFile}, 
		{AssetType::Model, Model::LoadModelAssetFromFile}
	};

	Ref<Asset> Asset::LoadAssetFromFile(AssetType type, const std::filesystem::path& path)
	{
		if (!s_FunctionMap.contains(type))
		{
			TGE_LOG_WARNING("no asset of that type has associated loader function");
			return nullptr;
		}

		Ref<Asset> asset = s_FunctionMap.at(type)(path);
		asset->_SetPath(path);

		return asset;
	}

	void Asset::_SetPath(const std::filesystem::path& path)
	{
		m_Path = path;
	}

	std::string_view GetAssetTypeString(AssetType type)
	{
		switch (type)
		{
		case TooGoodEngine::AssetType::None:	return "None";
		case TooGoodEngine::AssetType::Image:	return "Image";
		case TooGoodEngine::AssetType::Model:	return "Model";
		default:								return "None (INVALID)";
			break;
		}
	}

}