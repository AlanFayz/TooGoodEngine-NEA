#pragma once

#include "Asset.h"

#include "API/OpenGL/Texture2D.h"
#include "API/OpenGL/Program.h"

namespace TooGoodEngine {

	inline constexpr uint32_t g_NumberOfMipMaps = 7;
	inline constexpr uint32_t g_EnvironmentMapWidth = 1024;
	inline constexpr uint32_t g_EnvironmentMapHeight = 1024;
	
	class EnvironmentMap : public Asset
	{
	public:
		EnvironmentMap() = default;
		~EnvironmentMap() = default;

		inline OpenGL::Texture2D& GetTexture() { return m_Texture; }

		virtual const AssetType GetAssetType() const { return AssetType::EnvironmentMap; }
		static  const AssetType GetStaticAssetType() { return AssetType::EnvironmentMap; }

		//this will be taken in as an equirectanglar image (.hdr)
		static Ref<EnvironmentMap> LoadEnviromentMapAssetFromFile(const std::filesystem::path& path);

	private:
		static void _RenderCube(OpenGL::Program* program);

	private:
		OpenGL::Texture2D m_Texture;
	};

}