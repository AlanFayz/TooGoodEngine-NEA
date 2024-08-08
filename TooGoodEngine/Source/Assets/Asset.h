#pragma once

#include "Core/UUID.h"

#include <filesystem>

namespace TooGoodEngine {
	
	enum class AssetType
	{
		None = 0, //more to add
		Image,
		Model
	};

	class Asset
	{
	public:
		Asset() = default;
		Asset(UUID other) : m_UUID(other) {}

		void SetID(UUID other) { m_UUID = other; }

		virtual ~Asset() = default;
		virtual const AssetType GetAssetType() const = 0;
		//static const AssetType GetStaticAssetType() 
		 
		inline const UUID GetAssetID() const { return m_UUID; }

		static Ref<Asset> LoadAssetFromFile(AssetType type, const std::filesystem::path& path);

	private:
		UUID m_UUID;
	};

}