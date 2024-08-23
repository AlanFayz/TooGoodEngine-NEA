#pragma once

#include "Core/UUID.h"

#include <filesystem>

namespace TooGoodEngine {
	
	enum class AssetType
	{
		None = 0, 
		Image,
		Model, 
		EnvironmentMap, 
		Script
	};

	std::string_view GetAssetTypeString(AssetType type);
	AssetType GetAssetTypeFromString(const std::string& strType);

	class Asset
	{
	public:
		Asset() = default;
		Asset(UUID other) : m_UUID(other) {}

		void SetID(UUID other) { m_UUID = other; }

		virtual ~Asset() = default;
		virtual const AssetType GetAssetType() const = 0;
		 
		inline const std::filesystem::path& GetPath() const { return m_Path; }
		inline const UUID GetAssetID() const { return m_UUID; }

		static Ref<Asset> LoadAssetFromFile(AssetType type, const std::filesystem::path& path);

	private:
		void _SetPath(const std::filesystem::path& path);

	private:
		UUID m_UUID;
		std::filesystem::path m_Path;
	};

}