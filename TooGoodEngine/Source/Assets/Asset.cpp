#include "Asset.h"

#include <map>
#include <functional>

#include "Image.h"
#include "Model.h"
#include "EnviormentMap.h"

namespace TooGoodEngine {

	static Ref<Asset> LoadUntypedAsset(const std::filesystem::path& path) 
	{
		return nullptr;
	}

	using FunctionMap = std::map<AssetType, std::function<Ref<Asset>(const std::filesystem::path&)>>;
	
	static const FunctionMap s_FunctionMap
	{
		{AssetType::None,		   LoadUntypedAsset}, 
		{AssetType::Image,		   Image::LoadImageAssetFromFile}, 
		{AssetType::Model,		   Model::LoadModelAssetFromFile}, 
		{AssetType::EnviormentMap, EnviormentMap::LoadEnviromentMapAssetFromFile}
	};

	Ref<Asset> Asset::LoadAssetFromFile(AssetType type, const std::filesystem::path& path)
	{
		if (!s_FunctionMap.contains(type))
		{
			TGE_LOG_WARNING("no asset of that type has associated loader function");
			return nullptr;
		}

		Ref<Asset> asset = s_FunctionMap.at(type)(path);
		if(asset)
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
			case AssetType::None:		   return "None";
			case AssetType::Image:		   return "Image";
			case AssetType::Model:		   return "Model";
			case AssetType::EnviormentMap: return "EnviormentMap";
			default:					   return "None";
				break;
		}
	}

	AssetType GetAssetTypeFromString(const std::string& strType)
	{
		if (strType == "None")			return AssetType::None;
		if (strType == "Image")			return AssetType::Image;
		if (strType == "Model")			return AssetType::Model;
		if (strType == "EnviormentMap")	return AssetType::EnviormentMap;

		return AssetType::None;
	}

}